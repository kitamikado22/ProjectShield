// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/PSEquipmentData.h"
#include "Abilities/GameplayAbility.h"
#include "Input/PSInputConfig.h"
#include "PSEquipmentOnHandsData.generated.h"


/**
 * 手に付ける装備品のデータアセット
 */
UCLASS()
class PROJECTSHIELD_API UPSEquipmentOnHandsData : public UPSEquipmentData
{
	GENERATED_BODY()
	
public:

	/** 装備したときに付与されるアビリティマップ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|EquipmentOnHands")
	TMap<EPSAbilityInputID, TSoftClassPtr<UGameplayAbility>> AbilitySoftClassMap;

};
