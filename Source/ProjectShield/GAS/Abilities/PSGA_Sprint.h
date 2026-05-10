// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Utility/Observer/PSSubject.h"
#include "PSGA_StaminaCostBase.h"
#include "PSGA_Sprint.generated.h"

/**
 * ダッシュアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_Sprint : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_Sprint();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:

	/** ダッシュ条件を満たすスティックの閾値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	float SprintStickThreshold;

	/** ダッシュ中に消費するスタミナのためのGameplayEffect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> StaminaDrainEffect;
	FActiveGameplayEffectHandle DrainHandle;

	/** スタミナ変化イベント解除用 */
	TSharedPtr<IPSDisposable> StaminaUpdateDisposable;

	/** スティック入力イベント解除用 */
	TSharedPtr<IPSDisposable> InputMoveDisposable;

	/**	空中にいるイベント解除用 */
	TSharedPtr<IPSDisposable> AirDisposable;

protected:

	/** ダッシュ中にジャンプしたら発動するアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayAbility> SprintJumpAbilityClass;

	/** ストップモーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UAnimMontage> ToStopMontage;

	/** ダッシュから急に止まるときに呼ばれる */
	void ToStop();
};
