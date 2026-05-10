// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PSGA_StaminaCostBase.h"
#include "Utility/Observer/PSSubject.h"
#include "PSGA_Jump.generated.h"

/**
 * ジャンプアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_Jump : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_Jump();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 着地イベントの購読解除用 */
	TSharedPtr<IPSDisposable> LandedDisposable;

};
