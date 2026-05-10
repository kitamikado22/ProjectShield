// Fill out your copyright notice in the Description page of Project Settings.


#include "PSExecCalc_DamageFeedback.h"
#include "AbilitySystemComponent.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "GAS/Attributes/PSCombatAttributeSet.h"
#include "GAS/Attributes/PSBreakAttributeSet.h"
#include "GAS/PSGameplayTags.h"

struct PSDamageFeedbackStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetPoise);

	PSDamageFeedbackStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSCombatAttributeSet, Poise, Target, false, TargetPoise);
	}
};

static const PSDamageFeedbackStatics& DamageFeedbackStatics()
{
	static PSDamageFeedbackStatics DamageFeedbackStatics;
	return DamageFeedbackStatics;
}

UPSExecCalc_DamageFeedback::UPSExecCalc_DamageFeedback()
{
	RelevantAttributesToCapture.Add(DamageFeedbackStatics().TargetPoiseDef);
}

void UPSExecCalc_DamageFeedback::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	// フィードバックなのでSourceとTargetは逆になることに注意
	// ターゲット（攻撃した側）
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	// ソース（攻撃された側）
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// ターゲットの攻撃値を取得
	float TargetAttack = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_Attack);
	float TargetImpact = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_Impact);
	// ターゲットのの緩衝力を取得
	float TargetPoise = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageFeedbackStatics().TargetPoiseDef, FAggregatorEvaluateParameters(), TargetPoise);

	// ソースの跳ね返した衝撃力とブレイク率を取得
	float SourceReImpact = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_ReImpact);
	float SourceBreakRate = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_BreakRate);

	// ブレイク悪化の計算
	float OutBreakWorsen = (TargetImpact + SourceReImpact) * SourceBreakRate - TargetPoise;
	OutBreakWorsen = FMath::Max(0.f, OutBreakWorsen);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UPSBreakAttributeSet::GetWorsenAttribute(),
		EGameplayModOp::Additive, 
		OutBreakWorsen
	));
}