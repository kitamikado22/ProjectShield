// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PSGameplayAbility.h"
#include "PSGA_LockOn.generated.h"

/**
 * ロックオンを有効化したり無効化したりする
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_LockOn : public UPSGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPSGA_LockOn();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


};
