// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Input/PSInputConfig.h"
#include "Data/Item/PSEquipmentOnHandsData.h"
#include "PSPlayerEquipmentComponent.generated.h"

/**
 * プレイヤー専用の装備コンポーネント
 */
UCLASS()
class PROJECTSHIELD_API UPSPlayerEquipmentComponent : public UPSEquipmentComponent
{
	GENERATED_BODY()
	
public:

	UPSPlayerEquipmentComponent();

	/** 指定スロットに指定のアイテムを装備する */
	virtual void Equip(EEquipmentSlot Slot, FName ItemID) override;

	/** 装備解除 */
	virtual void RemoveEquipment(EEquipmentSlot Slot) override;

protected:

	/** 装備品についてるアビリティをオーナーに付与 */
	void GiveEquipmentAbility(UPSEquipmentOnHandsData* EquipmentData, EPSAbilityInputID AbilityInputID);

	/** 装備したときに付与されるアビリティのハンドルマップ */
	TMap<EPSAbilityInputID, FGameplayAbilitySpecHandle> EquipmentAbilityHandles;
};
