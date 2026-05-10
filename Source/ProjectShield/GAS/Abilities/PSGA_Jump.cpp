// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_Jump.h"
#include "Character/PSCharacterBase.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/PSGameplayTags.h"

UPSGA_Jump::UPSGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags = GetAssetTags();
	AssetTags.AddTag(PSGameplayTags::Ability_Jump);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(PSGameplayTags::State_Jump);
}

void UPSGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// スタミナ消費
	ApplyStaminaCost();

	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("OwnerCharacter is not valid"));
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 回転をロック
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;

	// ジャンプ
	Character->Jump();

	// 着地イベントを購読
	LandedDisposable = Character->GetOnLandedEvent()->Subscribe(this, [this](const FHitResult& Hit)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		});
}

void UPSGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->StopJumping();
	}

	// 着地イベントの購読を解除
	if (LandedDisposable.IsValid())
	{
		LandedDisposable->Dispose();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

