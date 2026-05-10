// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PSGameplayAbility.h"
#include "Utility/Observer/PSSubject.h"
#include "PSGA_HitReaction.generated.h"

/**
 * 攻撃を受けた際の反応アビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_HitReaction : public UPSGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPSGA_HitReaction();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 吹っ飛ばすときの係数 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|HitReaction")
	float LaunchMultiplier = 5.f;

	/** 吹っ飛び判定の閾値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|HitReaction")
	float HeavyHitThreshold = 500.f;

	/** 軽めのヒットモーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit Reaction")
	TObjectPtr<UAnimMontage> LightHitMontage;

	/** 重め（吹っ飛び）のヒットモーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit Reaction")
	TObjectPtr<UAnimMontage> HeavyHitMontage;

	/** 着地するときのイベント解除用 */
	TSharedPtr<IPSDisposable> LandDisposable;

protected:

	/** アニメーションが終了してそのまま終わるときに呼ばれる */
	UFUNCTION()
	void OnMontageFinished();

	/** 吹っ飛ばされて着地したときに呼ばれる */
	void OnLanded();
};
