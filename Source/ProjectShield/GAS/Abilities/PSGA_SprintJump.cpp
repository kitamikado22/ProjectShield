// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_SprintJump.h"
#include "Character/PSCharacterBase.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"

UPSGA_SprintJump::UPSGA_SprintJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags = GetAssetTags();
	AssetTags.AddTag(PSGameplayTags::Ability_SprintJump);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(PSGameplayTags::State_SprintJump);
}

void UPSGA_SprintJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DEBUG_TEST_MESSAGE(TEXT("Activating SprintJump Ability"));

	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("OwnerCharacter is not valid"));
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (not JumpLandMontage)
	{
		DEBUG_MESSAGE_ERROR(TEXT("JumpLandMontage is not set"));
		UE_LOG(LogTemp, Error, TEXT("JumpLandMontage is not set")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// 着地イベントを購読
	LandDisposable = Character->GetOnLandedEvent()->Subscribe(this, [this](const FHitResult& Hit)
		{
			ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
			// 着地したときにモーションを再生
			if (Character && JumpLandMontage)
			{
				Character->PlayAnimMontage(JumpLandMontage);
			}
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		});
}

void UPSGA_SprintJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 着地イベントの購読を解除
	if (LandDisposable.IsValid())
	{
		LandDisposable->Dispose();
	}

	// スタミナ自然回復遅延GameEffectを適用
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (StaminaRegenDelayEffectClass)
		{
			ASC->ApplyGameplayEffectToSelf(
				StaminaRegenDelayEffectClass.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}