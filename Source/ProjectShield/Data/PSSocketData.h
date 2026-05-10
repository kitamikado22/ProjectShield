// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Equipment/PSEquipmentType.h"
#include "PSSocketData.generated.h"

/**
 * ソケット名を管理するデータ 装備部位とソケット名を紐づける
 */
UCLASS()
class PROJECTSHIELD_API UPSSocketData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** スケルタルメッシュの名前 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Socket")
	FName SKMName;
	
	/** 装備スロットとソケット名を紐づけるマップデータ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Socket")
	TMap<EEquipmentSlot, FName> EquipmentSocketMap;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Socket", SKMName);
	}
};
