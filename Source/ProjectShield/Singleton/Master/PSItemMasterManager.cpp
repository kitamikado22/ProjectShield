// Fill out your copyright notice in the Description page of Project Settings.


#include "PSItemMasterManager.h"
#include "Engine/AssetManager.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Equipment/PSEquipmentBase.h"
#include "Data/Item/PSEquipmentData.h"

//UPSItemMasterManager* UPSItemMasterManager::GetInstance(const UObject* WorldContextObject)
//{
//	if (not WorldContextObject)
//	{
//		return nullptr;
//	}
//
//	UWorld* World = WorldContextObject->GetWorld();
//	if (not World)
//	{
//		return nullptr;
//	}
//
//	UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>();
//	if (not GameInstance)
//	{
//		return nullptr;
//	}
//
//	return GameInstance->GetSubsystem<UPSItemMasterManager>();
//}

TSharedPtr<FPSAssetLoadHandle> UPSItemMasterManager::GetOrLoadItemData(FName ItemId, TFunction<void(UPSItemData*)> OnLoadCompleted)
{
	return GetOrLoadData(ItemId, [OnLoadCompleted, ItemId](UObject* LoadedObject)
	{
		// アイテムデータにキャスト
		UPSItemData* ItemData = Cast<UPSItemData>(LoadedObject);
		if (ItemData == nullptr)
		{
			DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("This is not an item. Item ID is %s."), *ItemId.ToString()));
			UE_LOG(LogTemp, Error, TEXT("This is not an item. Item ID is %s."), *ItemId.ToString()); SET_LOG_PATH(Error);
			OnLoadCompleted(nullptr);
			return;
		}
		OnLoadCompleted(ItemData);
	});
}

TSharedPtr<FPSAssetLoadHandle> UPSItemMasterManager::GetOrLoadEquipmentData(FName ItemId, TFunction<void(UPSEquipmentData*)> OnLoadCompleted)
{
	return GetOrLoadData(ItemId, [OnLoadCompleted, ItemId, this](UObject* LoadedObject)
	{
		// アイテムデータから装備品データにキャスト
		UPSEquipmentData* EquipmentData = Cast<UPSEquipmentData>(LoadedObject);
		if (EquipmentData == nullptr)
		{
			DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("This is not an equipment. Item ID is %s."), *ItemId.ToString()));
			UE_LOG(LogTemp, Error, TEXT("This is not an equipment. Item ID is %s."), *ItemId.ToString()); SET_LOG_PATH(Error);
			OnLoadCompleted(nullptr);
			return;
		}
		OnLoadCompleted(EquipmentData);
	});
}


//void UPSItemMasterManager::Initialize(FSubsystemCollectionBase& Collection)
//{
//	// マスターデータをロードする前にパスを設定
//	SetMasterDataTableByPath(TEXT("/Game/Data/Item/DT_Item.DT_Item"));
//
//	Super::Initialize(Collection);
//}

//const FPSItemTableRow* UPSItemMasterManager::GetRowDataByID(FName ItemID) const
//{
//	if (MasterDataTable == nullptr)
//	{
//		DEBUG_MESSAGE_ERROR(TEXT("Master data table for Item does not exist"));
//		UE_LOG(LogTemp, Error, TEXT("Master data table for Item does not exist")); SET_LOG_PATH;
//		return nullptr;
//	}
//
//	if (const FPSItemTableRow* ItemTableRow = MasterDataTable->FindRow<FPSItemTableRow>(ItemID, TEXT("")))
//	{
//		return ItemTableRow;
//	}
//	else
//	{
//		DEBUG_MESSAGE_ERROR(TEXT("This ItemID was invalid."));
//		UE_LOG(LogTemp, Error, TEXT("This ItemID was invalid. ItemID is %s."), *ItemID.ToString()); SET_LOG_PATH;
//	}
//	
//	return nullptr;
//}


//void UPSItemMasterManager::LoadItemData(FName ItemID, TFunction<void(UPSItemData*)> OnCompleted)
//{
//	const FPSItemTableRow* ItemTableRow = GetRowDataByID(ItemID);
//	if (ItemTableRow == nullptr)
//	{
//		OnCompleted(nullptr);
//		return;
//	}
//
//	// ロードする対象のオブジェクトが設定されていないなら
//	if (ItemTableRow->ItemData.IsNull())
//	{
//		return;
//	}
//
//	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
//
//	// 非同期ロード
//	StreamableManager.RequestAsyncLoad(ItemTableRow->ItemData.ToSoftObjectPath(), [OnCompleted, ItemTableRow]()
//	{
//		OnCompleted(ItemTableRow->ItemData.Get());
//	});
//}


//void UPSItemMasterManager::LoadEquipmentClass(const UPSEquipmentData* EquipmentData, TFunction<void(UClass*)> OnLoadCompleted)
//{
//	// ロードする対象のクラス型が設定されていないなら
//	if (EquipmentData->EquipmentClass.IsNull())
//	{
//		DEBUG_MESSAGE_WARNING(TEXT("Equipment class not set."));
//		UE_LOG(LogTemp, Warning, TEXT("Equipment class not set. Equipment is %s."), *EquipmentData->ItemID.ToString());
//		return;
//	}
//
//	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
//
//	DEBUG_MESSAGE(TEXT("Equipment class load requested."));
//
//	// 非同期ロード
//	StreamableManager.RequestAsyncLoad(EquipmentData->EquipmentClass.ToSoftObjectPath(), [OnLoadCompleted, EquipmentData]() {
//		DEBUG_MESSAGE(TEXT("Equipment loading complete."));
//		OnLoadCompleted(EquipmentData->EquipmentClass.Get());
//	});
//}