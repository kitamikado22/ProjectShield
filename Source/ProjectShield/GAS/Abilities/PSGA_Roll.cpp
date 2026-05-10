// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_Roll.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Character/PSCharacterBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GAS/PSGameplayTags.h"

UPSGA_Roll::UPSGA_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags = GetAssetTags();
	AssetTags.AddTag(PSGameplayTags::Ability_Roll);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(PSGameplayTags::State_Roll);
}

void UPSGA_Roll::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData 
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// スタミナ消費
	ApplyStaminaCost();

	if (not RollMontage)
	{
		DEBUG_MESSAGE_WARNING(TEXT("RollMontage is not set"));
		UE_LOG(LogTemp, Warning, TEXT("RollMontage is not set")); SET_LOG_PATH(Warning);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// イベントを購読
	UAbilityTask_WaitGameplayEvent* StartEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PSGameplayTags::Event_Roll_Start, nullptr, false, false);
	StartEventTask->EventReceived.AddDynamic(this, &UPSGA_Roll::OnStartInvincible);
	StartEventTask->ReadyForActivation();

	// ローリングアニメーションを再生
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, RollMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPSGA_Roll::OnMontageFinished);
	//MontageTask->OnInterrupted.AddDynamic(this, &UPSGA_Roll::OnMontageFinished);
	//MontageTask->OnCancelled.AddDynamic(this, &UPSGA_Roll::OnMontageFinished);
	//MontageTask->OnBlendOut.AddDynamic(this, &UPSGA_Roll::OnMontageFinished);
	MontageTask->ReadyForActivation();
}

void UPSGA_Roll::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{

	// 無敵タグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Invincible);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPSGA_Roll::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPSGA_Roll::OnStartInvincible(FGameplayEventData Payload)
{

	// 無敵タグを付与
	AddDynamicTagToOwner(PSGameplayTags::State_Invincible);

	// タイマーをセットして無敵タグを削除
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, InvincibleTime);
	DelayTask->OnFinish.AddDynamic(this, &UPSGA_Roll::OnFinishInvincible);
	DelayTask->ReadyForActivation();
}

void UPSGA_Roll::OnFinishInvincible()
{

	// 無敵タグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Invincible);
}