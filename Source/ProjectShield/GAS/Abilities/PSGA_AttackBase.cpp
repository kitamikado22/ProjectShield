// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_AttackBase.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"
#include "Character/PSCharacterBase.h"
#include "Components/PSDefenseComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GAS/PSGameplayEffectContext.h"

UPSGA_AttackBase::UPSGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// ダメージを与えるGameplayEffectをロード
	static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageEffectFinder(TEXT("/Game/Blueprints/GAS/GameplayEffects/GE_Damage.GE_Damage_C"));
	if (DamageEffectFinder.Succeeded())
	{
		DamageEffectClass = DamageEffectFinder.Class;
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to find Damage GameplayEffect class"));
		UE_LOG(LogTemp, Error, TEXT("Failed to find Damage GameplayEffect class")); SET_LOG_PATH(Error);
	}
	// フィードバックを与えるGameplayEffectをロード
	static ConstructorHelpers::FClassFinder<UGameplayEffect> FeedbackEffectFinder(TEXT("/Game/Blueprints/GAS/GameplayEffects/GE_AttackFeedback.GE_AttackFeedback_C"));
	if (FeedbackEffectFinder.Succeeded())
	{
		FeedbackEffectClass = FeedbackEffectFinder.Class;
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to find Feedback GameplayEffect class"));
		UE_LOG(LogTemp, Error, TEXT("Failed to find Feedback GameplayEffect class")); SET_LOG_PATH(Error);
	}

}

void UPSGA_AttackBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData 
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DEBUG_MESSAGE(FString::Printf(TEXT("Activate Ability: %s."), *this->GetName()));
	UE_LOG(LogTemp, Log, TEXT("Activate Ability: %s."), *this->GetName());

	// 攻撃開始イベントを購読
	UAbilityTask_WaitGameplayEvent* StartEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PSGameplayTags::Event_Attack_Start, nullptr, false, false);
	StartEventTask->EventReceived.AddDynamic(this, &UPSGA_AttackBase::StartAttackTrace);
	StartEventTask->ReadyForActivation();

	// 攻撃終了イベントを購読
	UAbilityTask_WaitGameplayEvent* EndEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PSGameplayTags::Event_Attack_End, nullptr, false, false);
	EndEventTask->EventReceived.AddDynamic(this, &UPSGA_AttackBase::EndAttackTrace);
	EndEventTask->ReadyForActivation();

	// 予備動作の時間待機
	UAbilityTask_WaitDelay* WindupTask = UAbilityTask_WaitDelay::WaitDelay(this, WindupTime);
	WindupTask->OnFinish.AddDynamic(this, &UPSGA_AttackBase::StartAttack);
	WindupTask->ReadyForActivation();

	// 攻撃アニメーションを再生
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AttackMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPSGA_AttackBase::OnMontageFinished);
	MontageTask->ReadyForActivation();
}

void UPSGA_AttackBase::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	DEBUG_MESSAGE(FString::Printf(TEXT("End Ability: %s."), *this->GetName()));
	UE_LOG(LogTemp, Log, TEXT("End Ability: %s."), *this->GetName());

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);	
}

void UPSGA_AttackBase::OnWeaponHit(const FHitResult& HitResult)
{
	AActor* TargetActor = HitResult.GetActor();
	if (not TargetActor)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Hit Actor is null."));
		UE_LOG(LogTemp, Error, TEXT("Hit Actor is null.")); SET_LOG_PATH(Error);
		return;
	}
	AActor* OwningActor = GetOwningActorFromActorInfo();
	if (not OwningActor)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Owning Actor is null."));
		UE_LOG(LogTemp, Error, TEXT("Owning Actor is null.")); SET_LOG_PATH(Error);
		return;
	}

	APSCharacterBase* TargetCharacter = Cast<APSCharacterBase>(TargetActor);
	if (not TargetCharacter)
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("Hit Actor is not a valid character. It's name is %s."), *TargetActor->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor is not a valid character. It's name is %s."), *TargetActor->GetName()); SET_LOG_PATH(Warning);
		return;
	}

	// 当たったアクタと攻撃しているアクタが同じであるとき
	if (TargetActor == OwningActor)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (not TargetASC)
	{
		DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("OtherActor %s does not have an AbilitySystemComponent"), *TargetActor->GetName()));
		UE_LOG(LogTemp, Error, TEXT("OtherActor %s does not have an AbilitySystemComponent"), *TargetActor->GetName()); SET_LOG_PATH(Error);
		return;
	}
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (not SourceASC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Cannot find Source AbilitySystemComponent"));
		UE_LOG(LogTemp, Error, TEXT("Cannot find Source AbilitySystemComponent")); SET_LOG_PATH(Error);
		return;
	}

	// 攻撃がヒットしたキャラクターにDamageEffectを適用
	FGameplayEffectContextHandle DamageContextHandle = SourceASC->MakeEffectContext();
	DamageContextHandle.AddSourceObject(OwningActor);
	DamageContextHandle.AddHitResult(HitResult);
	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, DamageContextHandle);
	if (not DamageSpecHandle.IsValid())
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to create GameplayEffectSpec for DamageEffect"));
		UE_LOG(LogTemp, Error, TEXT("Failed to create GameplayEffectSpec for DamageEffect")); SET_LOG_PATH(Error);
		return;
	}
	// 攻撃値を設定
	DamageSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_Attack, AttackPower);
	DamageSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_Impact, ImpactPower);
	// ターゲットがどのように守ったか確定する
	EPSDefenseType DefenseType = TargetCharacter->GetDefenseComponent()->DecideDefenseType(HitResult);
	DamageSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_DefenseType, static_cast<float>(DefenseType));
	// GameplayEffectを適用
	TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());


	// 攻撃エフェクトのフィードバック値を取得
	FGameplayEffectContext* DamageContext = DamageContextHandle.Get();
	if (not DamageContext)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Damage GameplayEffectContext is null"));
		UE_LOG(LogTemp, Error, TEXT("Damage GameplayEffectContext is null")); SET_LOG_PATH(Error);
		return;
	}
	// ContextがFPSGameplayEffectContextであることを確認してからキャスト
	if (DamageContext->GetScriptStruct() != FPSGameplayEffectContext::StaticStruct())
	{
		DEBUG_MESSAGE_ERROR(TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext"));
		UE_LOG(LogTemp, Error, TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext")); SET_LOG_PATH(Error);
		return;
	}
	FPSGameplayEffectContext* PSContext = static_cast<FPSGameplayEffectContext*>(DamageContext);
	float ReImpact = PSContext->GenericDataMap.FindRef(PSGameplayTags::Data_ReImpact);
	float BreakRate = PSContext->GenericDataMap.FindRef(PSGameplayTags::Data_BreakRate);


	// オーナにフィードバックを与えるGameplayEffectを適用
	FGameplayEffectContextHandle FeedbackContextHandle = SourceASC->MakeEffectContext();
	FeedbackContextHandle.AddSourceObject(TargetActor);
	FGameplayEffectSpecHandle FeedbackSpecHandle = SourceASC->MakeOutgoingSpec(FeedbackEffectClass, 1, FeedbackContextHandle);
	if (not FeedbackSpecHandle.IsValid())
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to create GameplayEffectSpec for FeedbackEffect"));
		UE_LOG(LogTemp, Error, TEXT("Failed to create GameplayEffectSpec for FeedbackEffect")); SET_LOG_PATH(Error);
		return;
	}
	// パラメータを設定
	FeedbackSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_Attack, AttackPower);
	FeedbackSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_Impact, ImpactPower);
	FeedbackSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_ReImpact, ReImpact);
	FeedbackSpecHandle.Data->SetSetByCallerMagnitude(PSGameplayTags::Data_BreakRate, BreakRate);
	// GameplayEffectを適用
	SourceASC->ApplyGameplayEffectSpecToSelf(*FeedbackSpecHandle.Data.Get());

	DEBUG_MESSAGE(FString::Printf(TEXT("Applied DamageEffect to %s"), *TargetActor->GetName()));
	UE_LOG(LogTemp, Log, TEXT("Applied DamageEffect to %s"), *TargetActor->GetName());
}

void UPSGA_AttackBase::StartAttackTrace(FGameplayEventData Payload)
{
	// 実装なし
	DEBUG_MESSAGE_WARNING(TEXT("StartAttack called in base class. This should be overridden in child classes."));
	UE_LOG(LogTemp, Warning, TEXT("StartAttack called in base class. This should be overridden in child classes.")); SET_LOG_PATH(Warning);
}

void UPSGA_AttackBase::EndAttackTrace(FGameplayEventData Payload)
{
	// 実装なし
	DEBUG_MESSAGE_WARNING(TEXT("EndAttack called in base class. This should be overridden in child classes."));
	UE_LOG(LogTemp, Warning, TEXT("EndAttack called in base class. This should be overridden in child classes.")); SET_LOG_PATH(Warning);
}

void UPSGA_AttackBase::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPSGA_AttackBase::StartAttack()
{
	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection(FName("Execute"), AttackMontage);
		}
	}
}