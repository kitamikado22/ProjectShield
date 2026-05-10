// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PSGA_StaminaCostBase.h"
#include "Utility/Observer/PSSubject.h"
#include "PSGA_SprintJump.generated.h"

/**
 * ダッシュからジャンプしたときに発動するアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_SprintJump : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_SprintJump();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** ジャンプして着地するときのモーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UAnimMontage> JumpLandMontage;

	/** 着地するときのイベント解除用 */
	TSharedPtr<IPSDisposable> LandDisposable;
};
