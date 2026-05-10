// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/Abilities/PSGA_StaminaCostBase.h"
#include "PSGA_Parry.generated.h"

/**
 * パリィアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_Parry : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_Parry();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** パリィのモーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Parry")
	TObjectPtr<UAnimMontage> ParryMontage;

protected:

	/** アニメーションが終了した際に呼ばれる */
	UFUNCTION()
	void OnMontageFinished();
	/** パリィがスタートしたときに呼ばれる */
	UFUNCTION()
	void OnStartParry(FGameplayEventData Payload);
	/** パリィが終了したときに呼ばれる */
	UFUNCTION()
	void OnFinishedParry();
};
