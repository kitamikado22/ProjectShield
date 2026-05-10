// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/Observer/PSSubject.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Utility/PSDebugMessageMacross.h"
#include "PSAssetManager.generated.h"


/** FStreamableHandleのラップハンドル */
struct FPSAssetLoadHandle : public TSharedFromThis<FPSAssetLoadHandle>
{
	/** ロード完了時のコールバック関数を複数登録できる */
	TSharedPtr<TPSSubject<>> LoadEventSubject;

	TSharedPtr<FStreamableHandle> StreamableHandle;
};

/**
 * 既存のロードシステムを拡張するためのアセットマネージャークラス
 */
UCLASS()
class PROJECTSHIELD_API UPSAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	/** アセットマネージャーのインスタンスを取得 */
	inline static UPSAssetManager* Get()
	{
		return CastChecked<UPSAssetManager>(UAssetManager::GetIfInitialized());
	}

public:

	/** エンジン起動時に呼ばれる */
	virtual void StartInitialLoading() override;

	/** 単一のソフトオブジェクト非同期ロード */
	template<typename T>
	inline TSharedPtr<FPSAssetLoadHandle> RequestAsyncLoad(TSoftObjectPtr<T> SoftPtr, TFunction<void(UObject*)> OnLoadCompleted)
	{
		if (SoftPtr.IsNull())
		{
			DEBUG_MESSAGE_ERROR(TEXT("SoftPtr is null."));
			UE_LOG(LogTemp, Error, TEXT("SoftPtr is null.")); SET_LOG_PATH(Error);
			OnLoadCompleted(nullptr);
			return nullptr;
		}

		return UPSAssetManager::Get()->RequestAsyncLoad(SoftPtr.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([SoftPtr, OnLoadCompleted]()
			{
				UObject* LoadedObject = SoftPtr.Get();
				if (not LoadedObject)
				{
					DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Failed to load object. Object is %s."), *SoftPtr.GetAssetName()));
					UE_LOG(LogTemp, Error, TEXT("Failed to load object. Object is %s."), *SoftPtr.GetAssetName()); SET_LOG_PATH(Error);
					OnLoadCompleted(nullptr);
					return;
				}

				OnLoadCompleted(LoadedObject);
			}));
	}

	/** アセットを非同期ロード StreamableManagerのRequestAsyncLoadのラップ関数 */
	template< typename PathContainerType = TArray<FSoftObjectPath>, typename = std::enable_if_t<!std::is_same_v<std::decay_t<PathContainerType>, FStreamableAsyncLoadParams>, void> >
	inline TSharedPtr<FPSAssetLoadHandle> RequestAsyncLoad(
		PathContainerType&& TargetsToStream,
		FStreamableDelegate DelegateToCall = FStreamableDelegate(),
		TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		bool bManageActiveHandle = false,
		bool bStartStalled = false,
		FString DebugName = FString(),
		UE::FSourceLocation Location = UE::FSourceLocation::Current())
	{
		TSharedPtr<FPSAssetLoadHandle> NewHandle = MakeShared<FPSAssetLoadHandle>();
		NewHandle->LoadEventSubject = MakeShared<TPSSubject<>>();
		NewHandle->StreamableHandle = GetStreamableManager().RequestAsyncLoad(
			Forward<PathContainerType>(TargetsToStream),
			[NewHandle, Callback = MoveTemp(DelegateToCall)]()
			{
				if (Callback.IsBound())
				{
					Callback.Execute();
				}
				if (NewHandle->LoadEventSubject.IsValid())
				{
					NewHandle->LoadEventSubject->OnCompleted();
				}
			},
			Priority,
			bManageActiveHandle,
			bStartStalled,
			DebugName,
			Location
		);
		return NewHandle;
	}

	/** アセットIDからアセットを非同期ロード */
	inline TSharedPtr<FPSAssetLoadHandle> RequestLoadPrimaryAsset(
		const FPrimaryAssetId& AssetToLoad,
		const TArray<FName>& LoadBundles = TArray<FName>(),
		FStreamableDelegate DelegateToCall = FStreamableDelegate(),
		TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		UE::FSourceLocation Location = UE::FSourceLocation::Current())
	{
		TSharedPtr<FPSAssetLoadHandle> NewHandle = MakeShared<FPSAssetLoadHandle>();
		NewHandle->LoadEventSubject = MakeShared<TPSSubject<>>();
		NewHandle->StreamableHandle = LoadPrimaryAsset(
			AssetToLoad,
			LoadBundles,
			FStreamableDelegate::CreateLambda([NewHandle, DelegateToCall = MoveTemp(DelegateToCall)]()
				{
					if (DelegateToCall.IsBound())
					{
						DelegateToCall.Execute();
					}
					if (NewHandle->LoadEventSubject.IsValid())
					{
						NewHandle->LoadEventSubject->OnCompleted();
					}
				}),
			Priority,
			Location
		);
		return NewHandle;
	}
};
