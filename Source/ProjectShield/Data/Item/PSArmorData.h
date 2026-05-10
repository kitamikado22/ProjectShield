// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSEquipmentData.h"
#include "PSArmorData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHIELD_API UPSArmorData : public UPSEquipmentData
{
	GENERATED_BODY()

public:

	/** 防具の防御力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Armor")
	float DefensePower;
};
