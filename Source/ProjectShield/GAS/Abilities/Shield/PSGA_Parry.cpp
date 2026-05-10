// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_Parry.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemComponent.h"
#include "Character//PSCharacterBase.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Data/Item/PSShieldData.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"


UPSGA_Parry::UPSGA_Parry()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags = GetAssetTags();
	AssetTags.AddTag(PSGameplayTags::Ability_Parry);
	SetAssetTags(AssetTags);
}

void UPSGA_Parry::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (not ParryMontage)
	{
		DEBUG_MESSAGE_WARNING(TEXT("ParryMontage is not set"));
		UE_LOG(LogTemp, Warning, TEXT("ParryMontage is not set")); SET_LOG_PATH(Warning);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// パリィ開始イベントを購読
	UAbilityTask_WaitGameplayEvent* StartEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PSGameplayTags::Event_Parry_Start, nullptr, false, false);
	StartEventTask->EventReceived.AddDynamic(this, &UPSGA_Parry::OnStartParry);
	StartEventTask->ReadyForActivation();

	// パリィアニメーションを再生
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, ParryMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPSGA_Parry::OnMontageFinished);
	MontageTask->OnCancelled.AddDynamic(this, &UPSGA_Parry::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UPSGA_Parry::OnMontageFinished);
	MontageTask->ReadyForActivation();
}

void UPSGA_Parry::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	DEBUG_MESSAGE(TEXT("Parry Ability is End"));
	UE_LOG(LogTemp, Log, TEXT("Parry Ability is End"));

	if (UPSAbilitySystemComponent* PSASC = Cast<UPSAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		// パリィが成功している場合
		if (PSASC->HasMatchingGameplayTag(PSGameplayTags::State_Parry_Success))
		{
			PSASC->RemoveDynamicTagFromOwner(PSGameplayTags::State_Parry_Success);
		}
		// 成功していない場合
		else
		{
			// スタミナコストを適用
			ApplyStaminaCost();
		}
	}

	// スタミナ回復遅延を適用
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (StaminaRegenDelayEffectClass)
		{
			ASC->ApplyGameplayEffectToSelf(StaminaRegenDelayEffectClass.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());
		}
	}

	// パリィ状態タグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Parry);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPSGA_Parry::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPSGA_Parry::OnStartParry(FGameplayEventData Payload)
{

	// パリィ状態タグを付与
	AddDynamicTagToOwner(PSGameplayTags::State_Parry);

	// 現在使っている盾のデータを取得
	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("AvatarActor is not APSCharacterBase"));
		UE_LOG(LogTemp, Error, TEXT("AvatarActor is not APSCharacterBase")); SET_LOG_PATH(Error);
		return;
	}
	UPSShieldData* ShieldData = Cast<UPSShieldData>(Character->GetEquipmentComponent()->GetUsedEquipmentData());
	if (not ShieldData)
	{
		DEBUG_MESSAGE_WARNING(TEXT("Used equipment is not a shield"));
		UE_LOG(LogTemp, Warning, TEXT("Used equipment is not a shield")); SET_LOG_PATH(Warning);
		return;
	}

	// パリィ判定タイマーを開始
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShieldData->ParryTiming);
	DelayTask->OnFinish.AddDynamic(this, &UPSGA_Parry::OnFinishedParry);
	DelayTask->ReadyForActivation();
}

void UPSGA_Parry::OnFinishedParry()
{

	// パリィ状態タグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Parry);
}