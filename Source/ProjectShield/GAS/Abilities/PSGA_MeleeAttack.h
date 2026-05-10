// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Equipment/PSEquipmentType.h"
#include "PSGA_AttackBase.h"
#include "AbilitySystemComponent.h"
#include "PSGA_MeleeAttack.generated.h"


/**
 * 武器を使わない近接攻撃アビリティ
 */
UCLASS(Blueprintable)
class PROJECTSHIELD_API UPSGA_MeleeAttack : public UPSGA_AttackBase
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 攻撃を開始 */
	virtual void StartAttackTrace(FGameplayEventData Payload) override;

	/** 攻撃を終了 */
	virtual void EndAttackTrace(FGameplayEventData Payload) override;

	/** 攻撃中に武器が当たった際に呼ばれる */
	virtual void OnWeaponHit(const FHitResult& HitResult) override;

private:

	/** 攻撃トレースID */
	int32 AttackTraceID;
};
