// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PSGA_StaminaRegen.generated.h"

/**
 * スタミナを自然回復させるゲームプレイアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_StaminaRegen : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPSGA_StaminaRegen();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 回復用GameplayEffect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> StaminaRegenEffect;

	FActiveGameplayEffectHandle StaminaRegenEffectHandle;
};
