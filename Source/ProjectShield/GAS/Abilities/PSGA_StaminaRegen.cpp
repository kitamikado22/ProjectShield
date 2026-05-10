// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_StaminaRegen.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

UPSGA_StaminaRegen::UPSGA_StaminaRegen()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPSGA_StaminaRegen::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// GameplayEffectを適用してスタミナを自然回復
	if (ActorInfo->AbilitySystemComponent.IsValid() && StaminaRegenEffect)
	{
		StaminaRegenEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(
			StaminaRegenEffect.GetDefaultObject(), 1.0f, ActorInfo->AbilitySystemComponent->MakeEffectContext());
	}
}

void UPSGA_StaminaRegen::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// GameplayEffectを解除してスタミナの自然回復を止める
	if (ActorInfo->AbilitySystemComponent.IsValid() && StaminaRegenEffectHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(StaminaRegenEffectHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}