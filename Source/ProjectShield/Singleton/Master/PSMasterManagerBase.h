// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utility/Observer/PSSubject.h"
#include "Utility/PSAssetManager.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Singleton/PSGameInstanceSubsystem.h"
#include "PSMasterManagerBase.generated.h"


/**
 * ゲームのマスターデータ管理者
 */
UCLASS(Abstract)
class PROJECTSHIELD_API UPSMasterManagerBase : public UPSGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	/** キャッシュ */
	TMap<FName, TWeakObjectPtr<UObject>> CachedData;

public:

	/** 指定のIDからデータを取得またはロード */
	TSharedPtr<FPSAssetLoadHandle> GetOrLoadData(FName Id, TFunction<void(UObject*)> OnLoadCompleted);

	/** 指定のクラス型データを取得またはロード */
	template<typename T>
	TSharedPtr<FPSAssetLoadHandle> LoadClass(TSoftClassPtr<T>& TargetClass, TFunction<void(UClass*)> OnLoadCompleted)
	{
		// ロードしたいクラスが無効なら
		if (TargetClass.IsNull())
		{
			DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("TargetClass is null.")));
			UE_LOG(LogTemp, Error, TEXT("TargetClass is null.")); SET_LOG_PATH(Error);
			OnLoadCompleted(nullptr);
			return nullptr;
		}

		UPSAssetManager* AssetManager = UPSAssetManager::Get();

		// 非同期ロード
		auto AssetLoadHandle = AssetManager->RequestAsyncLoad(TargetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([&TargetClass, OnLoadCompleted]()
		{
			UClass* LoadedClass = TargetClass.Get();
			if (LoadedClass == nullptr)
			{
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Failed to load class. Class is %s."), *TargetClass.GetAssetName()));
				UE_LOG(LogTemp, Error, TEXT("Failed to load class. Class is %s."), *TargetClass.GetAssetName()); SET_LOG_PATH(Error);
				OnLoadCompleted(nullptr);
				return;
			}
			OnLoadCompleted(LoadedClass);
		}));
		return AssetLoadHandle;
	}

protected:

	/** カテゴリーごとに定義して、ゲーム全体で共通となるIDデータPrimaryAssetIdを作成 */
	virtual FORCEINLINE FPrimaryAssetId MakeAssetId(FName Id) const { return FPrimaryAssetId(); }

private:

	/** マスターでテーブルがロードされたことを通知するイベント */
	//TSharedRef<TPSSubject<>> OnLoadCompleted = TPSSubject<>::Create();

public:
	
	/** マスターでテーブルがロードされたことを通知するイベント取得 */
	//TSharedRef<IPSObservable<>> GetOnLoadCompleted() { return OnLoadCompleted->AsObservable(); }

protected:

	/** 使用するマスターデータテーブルのソフトポインタ */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|MasterData")
	//TSoftObjectPtr<UDataTable> DataTableSoftObject;

	/** 実際に使用するマスターデータ */
	//TObjectPtr<UDataTable> MasterDataTable;
	
public:

	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** 指定のIDから実際のデータを取得 */
	//virtual const FPSTableRowBase* GetRowDataByID(FName ID) const;

protected:

	/** 使用するデータテーブルをContent内のパスから設定 */
	//void SetMasterDataTableByPath(FString AssetPath);

	/** ロードが完了したときに呼ばれる */
	//virtual void OnCompletedLoadingMasterData();

private:

	/** マスターデータテーブルをロード */
	//void LoadMasterDataTable();
};
