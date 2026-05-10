// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PSGA_AttackBase.generated.h"

/**
 * 攻撃に関するアビリティの基底クラス
 */
UCLASS(Abstract)
class PROJECTSHIELD_API UPSGA_AttackBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPSGA_AttackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 攻撃力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attack")
	float AttackPower;

	/** 衝撃力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attack")
	float ImpactPower;

	/** 攻撃の当たり判定の大きさ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attack")
	float AttackRadius = 5.0f;

	/** 攻撃の根元のソケット名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attack")
	FName RootSocket = TEXT("Root");

	/** 攻撃の先端のソケット名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attack")
	FName TipSocket = TEXT("Tip");

protected:

	/** 予備動作の時間 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Timing")
	float WindupTime = 0.5f;

	/** ダメージを与えるGameplayEffect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GameEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** ダメージを与える際にフィードバック的にオーナに影響を及ぼすGameplayEffect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GameEffect")
	TSubclassOf<UGameplayEffect> FeedbackEffectClass;

	/** 攻撃に使用するアニメーション */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

protected:

	/** 攻撃を開始 */
	UFUNCTION()
	virtual void StartAttack();

	/** 攻撃トレースを開始 */
	UFUNCTION()
	virtual void StartAttackTrace(FGameplayEventData Payload);

	/** 攻撃を終了 */
	UFUNCTION()
	virtual void EndAttackTrace(FGameplayEventData Payload);

	/** 攻撃中に武器が当たった際に呼ばれる */
	virtual void OnWeaponHit(const FHitResult& HitResult);

	/** アニメーションが終了した際に呼ばれる */
	UFUNCTION()
	virtual void OnMontageFinished();
	
};
