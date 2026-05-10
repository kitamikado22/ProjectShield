// Fill out your copyright notice in the Description page of Project Settings.


#include "PSMultiLoadManager.h"

UPSMultiLoadManager* UPSMultiLoadManager::GetInstance(const UObject* WorldContextObject)
{
	if (not WorldContextObject)
	{
		return nullptr;
	}
	UWorld* World = WorldContextObject->GetWorld();
	if (not World)
	{
		return nullptr;
	}
	UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>();
	if (not GameInstance)
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UPSMultiLoadManager>();
}

void UPSMultiLoadManager::AddToLoadBlock(FName BlockName, FName LoaderName, TSharedPtr<FPSAssetLoadHandle> AssetLoadHandle)
{
	// ブロックが存在しなければ新規作成
	if (!ActiveLoadBlocks.Contains(BlockName))
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Creating new load block. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Log, TEXT("Creating new load block. Block name is %s."), *BlockName.ToString());
		ActiveLoadBlocks.Add(BlockName, MakeShared<FPSLoadBlock>());
	}

	TSharedPtr<FPSLoadBlock> LoadBlock = ActiveLoadBlocks[BlockName];

	// 同じ名前のロードが既に存在するなら追加しない
	if (LoadBlock->Loaders.Contains(LoaderName))
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("Loader already exists in block %s. Loader name is %s."), *BlockName.ToString(), *LoaderName.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("Loader already exists in block %s. Loader name is %s."), *BlockName.ToString(), *LoaderName.ToString()); SET_LOG_PATH(Warning);
		return;
	}

	// ロードを追加
	LoadBlock->Loaders.Add(LoaderName, AssetLoadHandle);

	if (AssetLoadHandle.IsValid())
	{
		// ロード完了時のコールバックを設定
		AssetLoadHandle->LoadEventSubject->Subscribe(this, nullptr, nullptr, [this, BlockName, LoaderName]()
		{
			DEBUG_MESSAGE(FString::Printf(TEXT("%s in the %s is complete."), *LoaderName.ToString(), *BlockName.ToString()));
			UE_LOG(LogTemp, Log, TEXT("%s in the %s is complete."), *LoaderName.ToString(), *BlockName.ToString());

			// ブロックが存在していなければ
			if (not ActiveLoadBlocks.Contains(BlockName))
			{
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Block does not exist. Block name is %s."), *BlockName.ToString()));
				UE_LOG(LogTemp, Error, TEXT("Block does not exist. Block name is %s."), *BlockName.ToString()); SET_LOG_PATH(Warning);
				return;
			}

			TSharedPtr<FPSLoadBlock> LoadBlock = ActiveLoadBlocks[BlockName];

			LoadBlock->CompletedLoaders.Add(LoaderName);	// ロードが完了したことを記録

			// ロード完了条件を満たすイベントを発行
			for (auto& Pair : LoadBlock->LoadBlockEventMap)
			{
				const TPSSet<FName>& WaitUntilLoaders = Pair.Key;
				const TSharedPtr<TPSSubject<>> LoadBlockEvent = Pair.Value;

				// 条件を満たしていればイベント発行
				if (LoadBlock->CompletedLoaders.Includes(WaitUntilLoaders.Elements))
				{
					LoadBlockEvent->OnCompleted();
					LoadBlock->UsedLoaders.Append(WaitUntilLoaders.Elements);
				}
			}

			// 全てのロードが完了していなければ
			if (LoadBlock->CompletedLoaders.Num() != LoadBlock->Loaders.Num())
			{
				return;
			}

			// 全てのイベントが完了しているか走査
			for (auto& Pair : LoadBlock->LoadBlockEventMap)
			{
				const TSharedPtr<TPSSubject<>> LoadBlockEvent = Pair.Value;

				if (not LoadBlockEvent->IsCompleted())
				{
					return;	// 完了していないイベントがある
				}
			}

			// 使用したロードの数と、完了したロードの数が異なれば
			if (LoadBlock->UsedLoaders.Num() != LoadBlock->CompletedLoaders.Num())
			{
				return;
			}
			// 使用されていないロードがあれば
			if (not LoadBlock->CompletedLoaders.Includes(LoadBlock->UsedLoaders))
			{
				return;
			}

			// 全てのロードが完了しており
			// 全てのイベントが完了しており
			// 全てのロードが一度でもイベントに使用されていれば
			ProcessEndLoadBlock(BlockName, LoadBlock);
		});
	}
}

TSharedPtr<IPSDisposable> UPSMultiLoadManager::SubscribeToLoadBlock(UObject* Owner, FName BlockName, TPSSet<FName> WaitUntilLoaders, TFunction<void()> OnLoadCompleted)
{
	// 完了条件が空なら即座に完了イベントを発行して終了
	if (WaitUntilLoaders.Elements.Num() == 0)
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("WaitUntilLoaders is empty. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("WaitUntilLoaders is empty. Block name is %s."), *BlockName.ToString()); SET_LOG_PATH(Warning);
		OnLoadCompleted();
		return nullptr;
	}

	// 履歴が使用できるなら即座にイベントを発行
	if (CanUseHistory(BlockName, WaitUntilLoaders.Elements))
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Trusted the history and used it. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Log, TEXT("Trusted the history and used it. Block name is %s."), *BlockName.ToString());
		OnLoadCompleted();
		return nullptr;
	}

	// ブロックが存在しなければ新規作成
	if (not ActiveLoadBlocks.Contains(BlockName))
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Creating new load block. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Log, TEXT("Creating new load block. Block name is %s."), *BlockName.ToString());
		ActiveLoadBlocks.Add(BlockName, MakeShared<FPSLoadBlock>());
	}	
	
	TSharedPtr<FPSLoadBlock> LoadBlock = ActiveLoadBlocks[BlockName];

	// 既に完了しているロードは条件から除外する
	for (const FName& CompletedLoader : LoadBlock->CompletedLoaders)
	{
		if (WaitUntilLoaders.Elements.Contains(CompletedLoader))
		{
			WaitUntilLoaders.Elements.Remove(CompletedLoader);
		}
	}
	// 既に条件を満たしているなら即座に完了イベントを発行
	if (WaitUntilLoaders.Elements.Num() == 0)
	{
		OnLoadCompleted();
		return nullptr;
	}

	// イベント作成
	if (not LoadBlock->LoadBlockEventMap.Contains(WaitUntilLoaders))
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Creating new load block event. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Log, TEXT("Creating new load block event. Block name is %s."), *BlockName.ToString());
		LoadBlock->LoadBlockEventMap.Add(WaitUntilLoaders, MakeShared<TPSSubject<>>());
	}

	TSharedPtr<TPSSubject<>> LoadEventSubject = LoadBlock->LoadBlockEventMap[WaitUntilLoaders];
	return LoadEventSubject->Subscribe(Owner, nullptr, nullptr, OnLoadCompleted);
}

inline void UPSMultiLoadManager::ProcessEndLoadBlock(FName BlockName, TSharedPtr<FPSLoadBlock> LoadBlock)
{
	// 履歴に保存
	LoadBlockHistories.Add(BlockName, FPSLoadBlockHistory());
	FPSLoadBlockHistory& NewHistory = LoadBlockHistories[BlockName];
	NewHistory.CompletedLoaders = LoadBlock->CompletedLoaders;
	for (const auto& Pair : LoadBlock->Loaders)
	{
		const FName& LoaderName = Pair.Key;
		const TSharedPtr<FPSAssetLoadHandle>& AssetLoadHandle = Pair.Value;
		if (AssetLoadHandle.IsValid() && AssetLoadHandle->StreamableHandle.IsValid())
		{
			NewHistory.WeakLoadedObjects.Add(LoaderName, AssetLoadHandle->StreamableHandle->GetLoadedAsset());
		}
	}

	// ブロックを削除
	ActiveLoadBlocks.Remove(BlockName);
	DEBUG_MESSAGE(FString::Printf(TEXT("Load block successfully completed. Block name is %s."), *BlockName.ToString()));
	UE_LOG(LogTemp, Log, TEXT("Load block successfully completed. Block name is %s."), *BlockName.ToString());
}

bool UPSMultiLoadManager::CanUseHistory(FName BlockName, const TSet<FName>& WaitUntilLoaders)
{
	if (not LoadBlockHistories.Contains(BlockName))
	{
		return false;
	}

	FPSLoadBlockHistory& History = LoadBlockHistories[BlockName];

	// 履歴に残っているロードに含まれていない条件であれば
	if (not History.CompletedLoaders.Includes(WaitUntilLoaders))
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("This history does not contain the specified loader. Block name is %s."), *BlockName.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("This history does not contain the specified loader. Block name is %s."), *BlockName.ToString()); SET_LOG_PATH(Warning);
		return false;
	}

	for (const auto& LoaderName : WaitUntilLoaders)
	{
		// ロード済みのデータがもう解放されていたら
		if (not History.WeakLoadedObjects[LoaderName].IsValid())
		{
			DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("%s is not valid. I will delete %s."), *LoaderName.ToString(), *BlockName.ToString()));
			UE_LOG(LogTemp, Warning, TEXT("%s is not valid. I will delete %s."), *LoaderName.ToString(), *BlockName.ToString()); SET_LOG_PATH(Warning);
			LoadBlockHistories.Remove(BlockName);
			return false;
		}
	}

	// 実際にデータがまだ残っていることが確認できれば履歴を信用
	return true;
}