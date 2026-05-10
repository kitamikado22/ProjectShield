// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_StaminaCostBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Character/PSCharacterBase.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"
#include "Animation/PSDefaultAnimInstance.h"

UPSGA_StaminaCostBase::UPSGA_StaminaCostBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	StaminaCostAmount = 10.f; // デフォルトのスタミナ消費量

	// スタミナ回復遅延のGameplayEffectをロード
	static ConstructorHelpers::FClassFinder<UGameplayEffect> StaminaRegenDelayEffectFinder(TEXT("/Game/Blueprints/GAS/GameplayEffects/GE_StaminaRegenDelay.GE_StaminaRegenDelay_C"));
	if (StaminaRegenDelayEffectFinder.Succeeded())
	{
		StaminaRegenDelayEffectClass = StaminaRegenDelayEffectFinder.Class;
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to find Stamina Regen Delay GameplayEffect class"));
		UE_LOG(LogTemp, Error, TEXT("Failed to find Stamina Regen Delay GameplayEffect class")); SET_LOG_PATH(Error);
	}

	ActivationOwnedTags.AddTag(PSGameplayTags::State_StaminaRegen_Blocked);
}

void UPSGA_StaminaCostBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// スタミナ回復遅延のGameplayEffectを適用
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (StaminaRegenDelayEffectClass)
		{
			ASC->ApplyGameplayEffectToSelf(
				StaminaRegenDelayEffectClass.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// アビリティが発動できるかチェック
	if (not CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// アニメーションの反転を設定
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (UPSDefaultAnimInstance* AnimInst = Cast<UPSDefaultAnimInstance>(Character->GetMesh()->GetAnimInstance()))
	{
		AnimInst->SetIsMirrored(bIsMirrored);
	}
}

void UPSGA_StaminaCostBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// アニメーションの反転を設定
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (UPSDefaultAnimInstance* AnimInst = Cast<UPSDefaultAnimInstance>(Character->GetMesh()->GetAnimInstance()))
	{
		AnimInst->SetIsMirrored(false);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UPSGA_StaminaCostBase::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (not Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	// スタミナが0より少しでもあればアビリティを発動できる
	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (Character && Character->GetStaminaAttributeSet()->GetStamina() > 0.f)
	{
		return true;
	}
	
	return false;
}

void UPSGA_StaminaCostBase::ApplyStaminaCost()
{
	if (APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		// スタミナを減らす
		Character->GetAbilitySystemComponent()->ApplyModToAttribute(
			Character->GetStaminaAttributeSet()->GetStaminaAttribute(),
			EGameplayModOp::Additive,
			-StaminaCostAmount);
	}
}
