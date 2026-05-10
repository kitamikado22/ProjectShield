// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Equipment/PSEquipmentType.h"
#include "PSGA_AttackBase.h"
#include "PSGA_WeaponAttack.generated.h"

/**
 * 武器を使った攻撃アビリティ
 */
UCLASS(Blueprintable)
class PROJECTSHIELD_API UPSGA_WeaponAttack : public UPSGA_AttackBase
{
	GENERATED_BODY()
	
//public:
//
//	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
//	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
//
//protected:
//
//	/** 攻撃に使用する装備スロット */
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Weapon")
//	EEquipmentSlot WeaponSlot;
//
//protected:
//
//	/** 攻撃中に武器が当たった際に呼ばれる */
//	virtual void OnWeaponHit(const FHitResult& HitResult) override;
};
