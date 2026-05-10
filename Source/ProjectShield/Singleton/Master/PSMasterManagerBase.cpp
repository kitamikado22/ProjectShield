// Fill out your copyright notice in the Description page of Project Settings.


#include "PSMasterManagerBase.h"
#include "Utility/PSDebugMessageMacross.h"

TSharedPtr<FPSAssetLoadHandle> UPSMasterManagerBase::GetOrLoadData(FName Id, TFunction<void(UObject*)> OnLoadCompleted)
{
	// できるならキャッシュデータを取得
	if (TWeakObjectPtr<UObject>* Found = CachedData.Find(Id))
	{
		if (Found->IsValid())
		{
			DEBUG_MESSAGE(FString::Printf(TEXT("Get data from cache. ID is %s."), *Id.ToString()));
			UE_LOG(LogTemp, Log, TEXT("Get data from cache. ID is %s."), *Id.ToString());
			OnLoadCompleted(Found->Get());
			return nullptr;
		}
	}

	FPrimaryAssetId AssetId = MakeAssetId(Id);

	UPSAssetManager* AssetManager = UPSAssetManager::Get();
	
	// AssetIDが存在しない場合
	if (AssetManager->GetPrimaryAssetPath(AssetId).IsNull())
	{
		DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Asset ID %s does not exist."), *AssetId.ToString()));
		UE_LOG(LogTemp, Error, TEXT("Asset ID %s does not exist."), *AssetId.ToString()); SET_LOG_PATH(Error);
		OnLoadCompleted(nullptr);
		return nullptr;
	}

	// 非同期ロード
	auto AssetLoadHandle = AssetManager->RequestLoadPrimaryAsset(
		AssetId,
		{},
		FStreamableDelegate::CreateLambda([this, AssetId, OnLoadCompleted]()
		{
			UObject* LoadedObject = UAssetManager::Get().GetPrimaryAssetObject(AssetId);
			if (LoadedObject == nullptr)
			{
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Failed to load asset. ID is %s."), *AssetId.ToString()));
				UE_LOG(LogTemp, Error, TEXT("Failed to load asset. ID is %s."), *AssetId.ToString()); SET_LOG_PATH(Error);
				OnLoadCompleted(nullptr);
				return;
			}

			// キャッシュに保存
			CachedData.Add(AssetId.PrimaryAssetName, LoadedObject);
			OnLoadCompleted(LoadedObject);
		})
	);
	return AssetLoadHandle;
}

//template<typename T>
//TSharedPtr<FPSAssetLoadHandle> UPSMasterManagerBase::LoadClass(TSoftClassPtr<T>& TargetClass, TFunction<void(UClass*)> OnLoadCompleted)
//{
//	// ロードしたいクラスが無効なら
//	if (TargetClass.IsNull())
//	{
//		OnLoadCompleted(nullptr);
//		return;
//	}
//
//	UPSAssetManager* AssetManager = UPSAssetManager::Get();
//
//	// 非同期ロード
//	auto AssetLoadHandle =  AssetManager->RequestAsyncLoad(
//		TargetClass.ToSoftObjectPath(),
//		FStreamableDelegate::CreateLambda([&TargetClass, OnLoadCompleted]()
//		{
//			UClass* LoadedClass = TargetClass.Get();
//			if (LoadedClass == nullptr)
//			{
//				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Failed to load class. Class is %s."), *TargetClass.GetAssetName()));
//				UE_LOG(LogTemp, Error, TEXT("Failed to load class. Class is %s."), *TargetClass.GetAssetName()); SET_LOG_PATH;
//				OnLoadCompleted(nullptr);
//				return;
//			}
//			OnLoadCompleted(LoadedClass);
//		})
//	);
//	return AssetLoadHandle;
//}


//void UPSMasterManagerBase::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	// マスターデータを非同期ロード
//	LoadMasterDataTable();	
//}
//
//void UPSMasterManagerBase::SetMasterDataTableByPath(FString AssetPath)
//{
//	DataTableSoftObject = FSoftObjectPath(AssetPath);
//}
//
//void UPSMasterManagerBase::LoadMasterDataTable()
//{
//	if (DataTableSoftObject.IsNull())
//	{
//		return;
//	}
//
//	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
//
//	// 非同期ロード
//	StreamableManager.RequestAsyncLoad(
//		DataTableSoftObject.ToSoftObjectPath(),
//		FStreamableDelegate::CreateUObject(this, &UPSMasterManagerBase::OnCompletedLoadingMasterData)
//	);
//}
//
//void UPSMasterManagerBase::OnCompletedLoadingMasterData()
//{
//	// ロード済みのマスターデータテーブルの実体を受け取る
//	MasterDataTable = DataTableSoftObject.Get();
//	if (MasterDataTable == nullptr)
//	{
//		DEBUG_MESSAGE_ERROR(TEXT("Master data table failed to load."));
//		UE_LOG(LogTemp, Error, TEXT("Master data table failed to load. Master is %s."), *this->GetName()); SET_LOG_PATH;
//
//		OnLoadCompleted->OnError(TEXT("Master data table failed to load."));
//
//		return;
//	}
//
//	DEBUG_MESSAGE(TEXT("Master data table loading completed."));
//
//	// ロードが完了したことを通知
//	OnLoadCompleted->OnCompleted();
//}
//
//const FPSTableRowBase* UPSMasterManagerBase::GetRowDataByID(FName ID) const
//{
//	return MasterDataTable->FindRow<FPSTableRowBase>(ID, TEXT("MasterData"));
//}