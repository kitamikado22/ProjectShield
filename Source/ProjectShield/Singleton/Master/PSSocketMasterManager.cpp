// Fill out your copyright notice in the Description page of Project Settings.


#include "PSSocketMasterManager.h"
#include "Data/PSSocketData.h"

//void UPSSocketMasterManager::Initialize(FSubsystemCollectionBase& Collection)
//{
//	// マスターデータをロードする前にパスを設定
//	SetMasterDataTableByPath(TEXT("/Game/Data/Socket/DT_EquipmentSocket.DT_EquipmentSocket"));
//
//	Super::Initialize(Collection);
//}

//UPSSocketMasterManager* UPSSocketMasterManager::GetInstance(const UObject* WorldContextObject)
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
//	return GameInstance->GetSubsystem<UPSSocketMasterManager>();
//}

//const FPSEquipmentSocketTableRow* UPSSocketMasterManager::GetRowDataByID(FName ID) const
//{
//	if (MasterDataTable == nullptr)
//	{
//		DEBUG_MESSAGE_ERROR(TEXT("Master data table does not exist"));
//		UE_LOG(LogTemp, Error, TEXT("Master data table does not exist")); SET_LOG_PATH;
//		return nullptr;
//	}
//
//	const FPSEquipmentSocketTableRow* SocketTableRow = MasterDataTable->FindRow<FPSEquipmentSocketTableRow>(ID, TEXT(""));
//
//	if (SocketTableRow == nullptr)
//	{
//		DEBUG_MESSAGE_ERROR(TEXT("This Skeletal Mesh Name was invalid."));
//		UE_LOG(LogTemp, Error, TEXT("This Skeletal Mesh Name was invalid. SMName is %s."), *ID.ToString()); SET_LOG_PATH;
//		return nullptr;
//	}
//
//	return SocketTableRow;
//}

TSharedPtr<FPSAssetLoadHandle> UPSSocketMasterManager::GetOrLoadEquipmentSocketMap(FName SKMName, TFunction<void(UPSSocketData*)> OnLoadCompleted)
{
	return GetOrLoadData(SKMName, [OnLoadCompleted, SKMName](UObject* LoadedObject)
	{
		// ソケットデータにキャスト
		UPSSocketData* SocketData = Cast<UPSSocketData>(LoadedObject);
		if (SocketData == nullptr)
		{
			DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("This is not a socket data. SKMName is %s."), *SKMName.ToString()));
			UE_LOG(LogTemp, Error, TEXT("This is not a socket data. SKMName is %s."), *SKMName.ToString()); SET_LOG_PATH(Error);
			OnLoadCompleted(nullptr);
			return;
		}
		OnLoadCompleted(SocketData);
	});
}