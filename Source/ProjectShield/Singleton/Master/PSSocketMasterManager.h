// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Singleton/Master/PSMasterManagerBase.h"
#include "PSSocketMasterManager.generated.h"

class UPSSocketData;

/**
 * キャラクターのソケット名などを管理
 * 装備スロットに対するソケット名のデータを保持している
 */
UCLASS()
class PROJECTSHIELD_API UPSSocketMasterManager : public UPSMasterManagerBase
{
	GENERATED_BODY()
	
public:

	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** 実際のインスタンスを取得 */
	//static UPSSocketMasterManager* GetInstance(const UObject* WorldContextObject);

	/** 指定IDの行データ取得 */
	//virtual const FPSEquipmentSocketTableRow* GetRowDataByID(FName ID) const override;

	/** スケルタルメッシュの名前から装備スロットとソケット名のマップデータを取得 */
	TSharedPtr<FPSAssetLoadHandle> GetOrLoadEquipmentSocketMap(FName SKMName, TFunction<void(UPSSocketData*)> OnLoadCompleted);

protected:

	virtual FORCEINLINE FPrimaryAssetId MakeAssetId(FName SKMName) const override { return FPrimaryAssetId("Socket", SKMName); }
};
