// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PSGA_StaminaCostBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "PSGA_Roll.generated.h"

/**
 * 回避用ローリングアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_Roll : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** ローリングアニメーションMontage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UAnimMontage> RollMontage;

	/** 無敵時間(s) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	float InvincibleTime = 0.5f;

protected:

	/** アニメーションが終了した際に呼ばれる */
	UFUNCTION()
	void OnMontageFinished();

	/** 無敵時間がスタートしたときに呼ばれる */
	UFUNCTION()
	void OnStartInvincible(FGameplayEventData Payload);

	/** 無敵時間が終了したときに呼ばれる */
	UFUNCTION()
	void OnFinishInvincible();
};
