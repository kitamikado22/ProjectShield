// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_HitReaction.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GAS/PSGameplayTags.h"
#include "GAS/PSGameplayEffectContext.h"
#include "Components/PSDefenseComponent.h"
#include "Character/PSCharacterBase.h"
#include "Utility/PSDebugMessageMacross.h"

UPSGA_HitReaction::UPSGA_HitReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(PSGameplayTags::State_Invincible);
	ActivationOwnedTags.AddTag(PSGameplayTags::State_DisableInput);
}

void UPSGA_HitReaction::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (not LightHitMontage || not HeavyHitMontage)
	{
		DEBUG_MESSAGE_ERROR(TEXT("The animation required for the hit reaction ability is not valid"));
		UE_LOG(LogTemp, Error, TEXT("The animation required for the hit reaction ability is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("OwnerCharacter is not valid"));
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (not ASC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("AbilitySystemComponent is not valid"));
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (not TriggerEventData)
	{
		DEBUG_MESSAGE_ERROR(TEXT("TriggerEventData is not valid"));
		UE_LOG(LogTemp, Error, TEXT("TriggerEventData is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// コンテキストデータが無効であれば
	if (not TriggerEventData->ContextHandle.IsValid())
	{
		DEBUG_MESSAGE_ERROR(TEXT("Context handle is not valid"));
		UE_LOG(LogTemp, Error, TEXT("Context handle is not valid")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 渡されたデータが一つもなければ例外終了
	if (TriggerEventData->TargetData.Num() == 0)
	{
		DEBUG_MESSAGE_ERROR(TEXT("None of the target data provided exists"));
		UE_LOG(LogTemp, Error, TEXT("None of the target data provided exists")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// イベントデータから衝撃力を取得
	const float InImpact = TriggerEventData->EventMagnitude;

	// TargetDataからヒットデータを取得
	const FHitResult* HitResult = TriggerEventData->TargetData.Get(0)->GetHitResult();
	FVector ImpactDirection = HitResult->ImpactNormal * -1.f;
	
	// 最終的に吹っ飛ばす速度ベクトル
	FVector ImpactVerocity = ImpactDirection * InImpact * LaunchMultiplier;

	// 衝撃力が強ければ、吹っ飛びモーション
	if (InImpact > HeavyHitThreshold)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HeavyHitMontage);
		MontageTask->OnCompleted.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->OnCancelled.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->OnInterrupted.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->ReadyForActivation();

		// 着地したときのイベント購読
		LandDisposable = Character->GetOnLandedEvent()->Subscribe(this, [this](const FHitResult& Hit)
			{
				OnLanded();
			});

		// 少し上方向に力を加える
		ImpactVerocity = ImpactVerocity + FVector(0.f, 0.f, 400.f);
	}
	// 衝撃力が弱ければ、軽いヒットモーション
	else
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LightHitMontage);
		MontageTask->OnCompleted.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->OnCancelled.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->OnInterrupted.AddDynamic(this, &UPSGA_HitReaction::OnMontageFinished);
		MontageTask->ReadyForActivation();
	}

	// 吹っ飛ばす
	Character->LaunchCharacter(ImpactVerocity, true, true);

	DEBUG_MESSAGE(FString::Printf(TEXT("Impact amount : %f  ImpactDirection(%f, %f, %f)"), ImpactVerocity.Size(), ImpactVerocity.X, ImpactVerocity.Y, ImpactVerocity.Z));
	UE_LOG(LogTemp, Log, TEXT("Impact amount : %f  ImpactDirection(%f, %f, %f)"), ImpactVerocity.Size(), ImpactVerocity.X, ImpactVerocity.Y, ImpactVerocity.Z);


	// コンテキストから守備方法を取得
	const FGameplayEffectContext* Context = TriggerEventData->ContextHandle.Get();
	if (not Context)
	{
		DEBUG_MESSAGE_ERROR(TEXT("GameplayEffectContext is null"));
		UE_LOG(LogTemp, Error, TEXT("GameplayEffectContext is null")); SET_LOG_PATH(Error);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	if (Context->GetScriptStruct() != FPSGameplayEffectContext::StaticStruct())
	{
		DEBUG_MESSAGE_ERROR(TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext"));
		UE_LOG(LogTemp, Error, TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext")); SET_LOG_PATH(Error);
		return;
	}
	const FPSGameplayEffectContext* PSContext = static_cast<const FPSGameplayEffectContext*>(Context);
	EPSDefenseType DefenseType = static_cast<EPSDefenseType>(PSContext->GenericDataMap.FindRef(PSGameplayTags::Data_DefenseType));

	// 演出を組む
	FGameplayCueParameters Params;
	Params.Location = HitResult->ImpactPoint;
	Params.Normal = HitResult->ImpactNormal;
	Params.EffectContext = TriggerEventData->ContextHandle;

	// 生身で攻撃を受けている場合
	if (DefenseType == EPSDefenseType::None)
	{
		ASC->ExecuteGameplayCue(PSGameplayTags::GameplayCue_Combat_Hit_Bleed, Params);
	}
	// パリィ
	else if (DefenseType == EPSDefenseType::Parry)
	{
		ASC->ExecuteGameplayCue(PSGameplayTags::GameplayCue_Combat_Hit_Parry, Params);
	}
	// 何かしら盾を使って防いでいた場合
	else
	{
		ASC->ExecuteGameplayCue(PSGameplayTags::GameplayCue_Combat_Hit_Spark, Params);
	}
	
}

void UPSGA_HitReaction::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPSGA_HitReaction::OnLanded()
{
	// 着地イベントの購読を解除
	if (LandDisposable.IsValid())
	{
		LandDisposable->Dispose();
	}

	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection(FName("Landed"), HeavyHitMontage);
		}
	}
}

void UPSGA_HitReaction::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	// 着地イベントの購読を解除
	if (LandDisposable.IsValid())
	{
		LandDisposable->Dispose();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}