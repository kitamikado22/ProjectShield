// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSItemData.h"
#include "Equipment/PSEquipmentType.h"
#include "Equipment/PSEquipmentBase.h"
#include "PSEquipmentData.generated.h"

/**
 * 装備品のデータ
 */
UCLASS()
class PROJECTSHIELD_API UPSEquipmentData : public UPSItemData
{
	GENERATED_BODY()
	
public:

	/** 装備できる部位 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Equipment")
	TArray<EEquipmentSlot> EquippableSlots;

	/** 装備したときのオフセット */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Equipment")
	TMap<EEquipmentSlot, FTransform> EquipOffsets;

	/** 実体化する装備品のアクタのクラス */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Equipment")
	TSoftClassPtr<APSEquipmentBase> EquipmentClass;

	/** 装備品の重さ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Equipment")
	float Weight;
};
