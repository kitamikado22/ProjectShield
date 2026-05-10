// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSEquipmentData.h"
#include "PSWeaponData.generated.h"

/**
 * 武器のデータ
 */
UCLASS()
class PROJECTSHIELD_API UPSWeaponData : public UPSEquipmentData
{
	GENERATED_BODY()
	
public:

	/** 武器の攻撃力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Weapon")
	float AttackPower;

};
