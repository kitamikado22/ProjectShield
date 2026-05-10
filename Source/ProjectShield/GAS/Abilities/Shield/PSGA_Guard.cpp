// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_Guard.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystemComponent.h"
#include "Character//PSCharacterBase.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Data/Item/PSShieldData.h"
#include "Data/Character/PSCharacterData.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"

UPSGA_Guard::UPSGA_Guard()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	StaminaCostAmount = 0.f;

	FGameplayTagContainer AssetTags = GetAssetTags();
	AssetTags.AddTag(PSGameplayTags::Ability_Guard);
	SetAssetTags(AssetTags);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> MoveSpeedEffectFinder(TEXT("/Game/Blueprints/GAS/GameplayEffects/GE_MoveSpeed.GE_MoveSpeed_C"));
	if (MoveSpeedEffectFinder.Succeeded())
	{
		MoveSpeedEffectClass = MoveSpeedEffectFinder.Class;
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to find MoveSpeed GameplayEffect class"));
		UE_LOG(LogTemp, Error, TEXT("Failed to find MoveSpeed GameplayEffect class")); SET_LOG_PATH(Error);
	}
}

void UPSGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	// フラグ初期化
	bDeflectFinished = false;
	bInputReleased = false;

	APSCharacterBase* Character = Cast<APSCharacterBase>(ActorInfo->AvatarActor.Get());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("AvatarActor is not APSCharacterBase"));
		UE_LOG(LogTemp, Error, TEXT("AvatarActor is not APSCharacterBase")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 受け流し開始イベントを購読
	UAbilityTask_WaitGameplayEvent* StartEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PSGameplayTags::Event_Deflect_Start, nullptr, false, false);
	StartEventTask->EventReceived.AddDynamic(this, &UPSGA_Guard::OnStartDeflect);
	StartEventTask->ReadyForActivation();

	// ガード中は歩きの速度にする
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(MoveSpeedEffectClass);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(
		PSGameplayTags::Data_MoveSpeed,
		Character->GetCharacterData()->WalkSpeed
	);
	MoveSpeedEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);

	// 入力が離されたときにガードを終了する
	UAbilityTask_WaitInputRelease* InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	InputReleaseTask->OnRelease.AddDynamic(this, &UPSGA_Guard::OnInputReleased);
	InputReleaseTask->ReadyForActivation();

	// 受け流しモーションを再生
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeflectMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPSGA_Guard::OnMontageFinished);
	MontageTask->ReadyForActivation();

}

void UPSGA_Guard::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	DEBUG_MESSAGE(TEXT("Guard Ability is End"));
	UE_LOG(LogTemp, Log, TEXT("Guard Ability is End"));

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		// スタミナ回復遅延を適用
		if (StaminaRegenDelayEffectClass)
		{
			ASC->ApplyGameplayEffectToSelf(StaminaRegenDelayEffectClass.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());
		}

		// ガード中の移動速度減少のGameplayEffectを解除
		if (MoveSpeedEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(MoveSpeedEffectHandle);
		}
	}

	// 受け流し状態のタグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Deflect);
	// ガード状態のタグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Guard);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPSGA_Guard::OnStartDeflect(FGameplayEventData Payload)
{

	// 受け流し状態のタグを付与
	AddDynamicTagToOwner(PSGameplayTags::State_Deflect);

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

	// 受け流し時間分待つ
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShieldData->DeflectTiming);
	DelayTask->OnFinish.AddDynamic(this, &UPSGA_Guard::OnFinishedDeflect);
	DelayTask->ReadyForActivation();
}

void UPSGA_Guard::OnFinishedDeflect()
{

	bDeflectFinished = true;

	// 受け流し状態のタグを削除
	RemoveDynamicTagFromOwner(PSGameplayTags::State_Deflect);

	// 既に入力が離されているときは、ガード状態に移行せずにアビリティを終了する
	if (bInputReleased)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// ガード状態に移行
	EnterGuardState();
}

void UPSGA_Guard::OnInputReleased(float TimeHeld)
{
	bInputReleased = true;

	// 受け流しが完了する前に入力が離されたときは、受け流しが完了するまで見送る
	if (not bDeflectFinished)
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPSGA_Guard::EnterGuardState()
{

	// ガード状態のタグを付与
	AddDynamicTagToOwner(PSGameplayTags::State_Guard);
}

void UPSGA_Guard::OnMontageFinished()
{
	// 既に入力が離されているとき
	//if (bInputReleased)
	//{
	//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	//	return;
	//}
}