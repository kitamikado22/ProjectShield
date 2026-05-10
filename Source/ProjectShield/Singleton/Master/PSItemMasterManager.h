// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Singleton/Master/PSMasterManagerBase.h"
#include "PSItemMasterManager.generated.h"

class UPSItemData;
class UPSEquipmentData;
class APSEquipmentBase;

/**
 * アイテムのマスターデータを管理する
 */
UCLASS()
class PROJECTSHIELD_API UPSItemMasterManager : public UPSMasterManagerBase
{
	GENERATED_BODY()

private:

	//TMap<FName, TWeakObjectPtr<UPSItemData>> CachedItemData;

public:

	/** 実際のインスタンスを取得 */
	static UPSItemMasterManager* GetInstance(const UObject* WorldContextObject);

	/** 指定のIDのアイテムデータを取得またはロード */
	TSharedPtr<FPSAssetLoadHandle> GetOrLoadItemData(FName ItemID, TFunction<void(UPSItemData*)> OnLoadCompleted);

	/** 指定のIDの装備品データをロードして提供 */
	TSharedPtr<FPSAssetLoadHandle> GetOrLoadEquipmentData(FName ItemID, TFunction<void(UPSEquipmentData*)> OnCompleted);

	/** 指定のIDからマスターデータの行データを取得 */
	//virtual const FPSItemTableRow* GetRowDataByID(FName ItemID) const override;

	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** 指定のIDのアイテムデータをマスターデータからロードして取得 */
	//void LoadItemData(FName ItemID, TFunction<void(UPSItemData*)> OnCompleted);

	/** 装備品のアクタのクラスをロード */
	//void LoadEquipmentClass(const UPSEquipmentData* EquipmentData, TFunction<void(UClass*)> OnCompleted);

protected:

	virtual FORCEINLINE FPrimaryAssetId MakeAssetId(FName ItemId) const override { return FPrimaryAssetId("Item", ItemId); }
};
