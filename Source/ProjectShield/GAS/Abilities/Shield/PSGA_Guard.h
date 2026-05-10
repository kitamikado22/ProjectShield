// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/Abilities/PSGA_StaminaCostBase.h"
#include "PSGA_Guard.generated.h"

/**
 * 受け流し、受け止めアビリティ
 */
UCLASS()
class PROJECTSHIELD_API UPSGA_Guard : public UPSGA_StaminaCostBase
{
	GENERATED_BODY()
	
public:

	UPSGA_Guard();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** 受け流しモーション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UAnimMontage> DeflectMontage;

	/** 速度を変更するGameplayEffect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> MoveSpeedEffectClass;
	FActiveGameplayEffectHandle MoveSpeedEffectHandle;

	/** 受け流しがスタートしたときに呼ばれる */
	UFUNCTION()
	void OnStartDeflect(FGameplayEventData Payload);

	/** 受け流しが終了したときに呼ばれる */
	UFUNCTION()
	void OnFinishedDeflect();

	/** 入力が離されたときに呼ばれる */
	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	/** 受け流しのアニメーションが終了したときに呼ばれる */
	UFUNCTION()
	void OnMontageFinished();

	/** 受け止め状態に入る 受け止め状態での処理を記述 */
	void EnterGuardState();

private:

	/** 受け流し完了したかどうか */
	bool bDeflectFinished = false;

	/** 受け流しが完了する前に入力が離されたか */
	bool bInputReleased = false;

};
