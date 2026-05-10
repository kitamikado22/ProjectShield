// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/Abilities/PSGameplayAbility.h"
#include "PSGA_StaminaCostBase.generated.h"

class APSCharacterBase;

/**
 * スタミナを消費するゲームプレイアビリティの基底クラス
 */
UCLASS(abstract)
class PROJECTSHIELD_API UPSGA_StaminaCostBase : public UPSGameplayAbility
{
	GENERATED_BODY()

public:

	UPSGA_StaminaCostBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

protected:

	/** アニメーションを反転するか */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	bool bIsMirrored;

	/** 消費するスタミナ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	float StaminaCostAmount;

	/** スタミナ回復遅延のGameplayEffectクラス */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> StaminaRegenDelayEffectClass;

protected:

	/** スタミナコストを支払う */
	void ApplyStaminaCost();
};
