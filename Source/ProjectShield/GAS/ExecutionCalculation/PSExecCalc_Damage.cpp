// Fill out your copyright notice in the Description page of Project Settings.


#include "PSExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "Character/PSCharacterBase.h"
#include "GAS/Attributes/PSCombatAttributeSet.h"
#include "GAS/Attributes/PSHealthAttributeSet.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "GAS/Attributes/PSBreakAttributeSet.h"
#include "GAS/Attributes/PSStatusAttributeSet.h"
#include "GAS/PSGameplayTags.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Equipment/PSEquipmentType.h"
#include "Data/Item/PSEquipmentData.h"
#include "Data/Item/PSShieldData.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Components/PSDefenseComponent.h"
#include "GAS/PSGameplayEffectContext.h"

struct PSDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetPoise);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetStrength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetDurability);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TargetStamina);

	DECLARE_ATTRIBUTE_CAPTUREDEF(SourcePoise);

	PSDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSCombatAttributeSet, Defense, Target, false, TargetDefense);
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSCombatAttributeSet, Poise, Target, false, TargetPoise);
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSStatusAttributeSet, Strength, Target, false, TargetStrength);
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSStatusAttributeSet, Durability, Target, false, TargetDurability);
		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSStaminaAttributeSet, Stamina, Target, false, TargetStamina);

		DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(UPSCombatAttributeSet, Poise, Source, false, SourcePoise);
	}
};

static const PSDamageStatics& DamageStatics()
{
	static PSDamageStatics DamageStatics;
	return DamageStatics;
}

UPSExecCalc_Damage::UPSExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().TargetDefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetPoiseDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetStrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetDurabilityDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetStaminaDef);

	RelevantAttributesToCapture.Add(DamageStatics().SourcePoiseDef);
}

void UPSExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (not TargetASC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("TargetASC is null"));
		UE_LOG(LogTemp, Error, TEXT("TargetASC is null")); SET_LOG_PATH(Error);
		return;
	}
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// ターゲットのタグを取得
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 無敵状態であれば終了
	if (TargetTags->HasTag(PSGameplayTags::State_Invincible))
	{
		DEBUG_MESSAGE(TEXT("Target Actor's state is invincible"));
		UE_LOG(LogTemp, Log, TEXT("Target Actor's state is invincible"));
		return;
	}

	APSCharacterBase* TargetCharacter = Cast<APSCharacterBase>(TargetASC->GetAvatarActor());
	if (not TargetCharacter)
	{
		DEBUG_MESSAGE_ERROR(TEXT("TargetActor is not of type APSCharacterBase"));
		UE_LOG(LogTemp, Error, TEXT("TargetActor is not of type APSCharacterBase")); SET_LOG_PATH(Error);
		return;
	}

	

	float BreakRate = 0.f;	// 計算結果をブレイクに反映させる割合

	// ターゲットの属性値を取得
	float TargetDefense = 0.f;		// 防御力
	float TargetPoise = 0.f;		// 緩衝力
	float TargetStrength = 0.f;		// 筋力
	float TargetDurability = 0.f;	// 耐久力
	float TargetStamina = 0.f;		// スタミナ
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().TargetDefenseDef, FAggregatorEvaluateParameters(), TargetDefense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().TargetPoiseDef, FAggregatorEvaluateParameters(), TargetPoise);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().TargetStrengthDef, FAggregatorEvaluateParameters(), TargetStrength);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().TargetDurabilityDef, FAggregatorEvaluateParameters(), TargetDurability);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().TargetStaminaDef, FAggregatorEvaluateParameters(), TargetStamina);

	// ソースの攻撃値を取得
	float SourceAttack = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_Attack);
	float SourceImpact = Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_Impact);
	// ソースの緩衝力を取得
	float SourcePoise = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().SourcePoiseDef, FAggregatorEvaluateParameters(), SourcePoise);

	// ターゲットの防御手段を取得
	EPSDefenseType DefenseType = (EPSDefenseType)(Spec.GetSetByCallerMagnitude(PSGameplayTags::Data_DefenseType));

	// ターゲットの使っている装備データを取得
	bool bUsedShield = false;
	if (UPSEquipmentData* EquipmentData = TargetCharacter->GetEquipmentComponent()->GetUsedEquipmentData())
	{
		// 装備している盾データを取得
		if (UPSShieldData* ShieldData = Cast<UPSShieldData>(EquipmentData))
		{
			if (DefenseType != EPSDefenseType::None)
			{
				TargetDefense += ShieldData->Defense;
				TargetPoise += ShieldData->Poise;
			}

			switch (DefenseType)
			{
				// 失敗している場合
			case EPSDefenseType::None:
				break;

				// 盾で受け止めている場合
			case EPSDefenseType::Guard:
				TargetDefense += ShieldData->GuardCorrection.DefenceCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->GuardCorrection.DefenceCorrection.DurabilityCoefficient * TargetDurability;
				TargetPoise += ShieldData->GuardCorrection.PoiseCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->GuardCorrection.PoiseCorrection.DurabilityCoefficient * TargetDurability;
				BreakRate = ShieldData->GuardBreakRate;
				bUsedShield = true;
				break;

				// 受け流している場合
			case EPSDefenseType::Deflect:
				TargetDefense += ShieldData->DeflectCorrection.DefenceCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->DeflectCorrection.DefenceCorrection.DurabilityCoefficient * TargetDurability;
				TargetPoise += ShieldData->DeflectCorrection.PoiseCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->DeflectCorrection.PoiseCorrection.DurabilityCoefficient * TargetDurability;
				BreakRate = ShieldData->DeflectBreakRate;
				bUsedShield = true;
				break;

				// 受け返している場合
			case EPSDefenseType::Parry:
				TargetDefense += ShieldData->ParryCorrection.DefenceCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->ParryCorrection.DefenceCorrection.DurabilityCoefficient * TargetDurability;
				TargetPoise += ShieldData->ParryCorrection.PoiseCorrection.StrengthCoefficient * TargetStrength
					+ ShieldData->ParryCorrection.PoiseCorrection.DurabilityCoefficient * TargetDurability;
				BreakRate = ShieldData->ParryBreakRate;
				bUsedShield = true;
				break;
			}			
		}
	}

	// ターゲットへの出力計算
	float OutDamage = SourceAttack - TargetDefense;	// ダメージ量
	float OutImpact = SourceImpact - TargetPoise;	// 衝撃量
	OutDamage = FMath::Max(OutDamage, 0.f);
	OutImpact = FMath::Max(OutImpact, 0.f);

	// 盾を使っている場合
	float OutStaminaDamage = 0.f;
	bool bFailedToUseShield = false;
	if (bUsedShield)
	{
		// スタミナ消費量（仮に衝撃量と同じとする）
		OutStaminaDamage = OutImpact;

		// 守るためのスタミナが足りない場合
		if (TargetStamina - OutStaminaDamage <= 0.f)
		{
			OutDamage = SourceAttack;
			OutImpact = SourceImpact;
			bFailedToUseShield = true;
		}
	}
	// パリィが成功している場合
	if (DefenseType == EPSDefenseType::Parry && not bFailedToUseShield)
	{
		// ターゲットにパリィ成功のタグを付与
		if (UPSAbilitySystemComponent* TargetPSASC = Cast<UPSAbilitySystemComponent>(TargetASC))
		{
			TargetPSASC->AddDynamicTagToOwner(PSGameplayTags::State_Parry_Success);
		}		
	}

	// ターゲットの属性に出力を反映
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UPSHealthAttributeSet::GetDamageAttribute(),
			EGameplayModOp::Additive,
			OutDamage
		));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UPSBreakAttributeSet::GetWorsenAttribute(),
			EGameplayModOp::Additive,
			OutImpact
		));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UPSStaminaAttributeSet::GetStaminaAttribute(),
			EGameplayModOp::Additive,
			-OutStaminaDamage
	));

	// 外部へデータを渡す
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	if (ContextHandle.IsValid())
	{
		if (FGameplayEffectContext* Context = ContextHandle.Get())
		{
			// ContextがFPSGameplayEffectContextであることを確認してからキャスト
			if (Context->GetScriptStruct() != FPSGameplayEffectContext::StaticStruct())
			{
				DEBUG_MESSAGE_ERROR(TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext"));
				UE_LOG(LogTemp, Error, TEXT("GameplayEffectContext is not of type FPSGameplayEffectContext")); SET_LOG_PATH(Error);
				return;
			}

			FPSGameplayEffectContext* PSContext = static_cast<FPSGameplayEffectContext*>(Context);
			PSContext->GenericDataMap.Add(PSGameplayTags::Data_ReImpact, TargetPoise);
			PSContext->GenericDataMap.Add(PSGameplayTags::Data_BreakRate, BreakRate);
			PSContext->GenericDataMap.Add(PSGameplayTags::Data_DefenseType, static_cast<float>(DefenseType));
		}
		
	}

	


	//// ソースへの出力計算
	//float OutBreakWorsen = (SourceImpact + TargetPoise) * BreakRate - SourcePoise;	// ブレイク悪化量
	//OutBreakWorsen = FMath::Max(OutBreakWorsen, 0.f);

	//// ソースの属性に出力を反映
	//if (SourceASC)
	//{
	//	SourceASC->ApplyModToAttribute(
	//		UPSBreakAttributeSet::GetWorsenAttribute(),
	//		EGameplayModOp::Additive,
	//		OutBreakWorsen
	//	);
	//}

	// Contextを通して値を外部に渡す
	//FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	//if (FPSGameplayEffectContext* PSContext = static_cast<FPSGameplayEffectContext*>(ContextHandle.Get()))
	//{
	//	PSContext->KnockbackDirection = PSContext->GetHitResult() ? PSContext->GetHitResult()->ImpactNormal : FVector::ZeroVector;	// 衝撃方向
	//	PSContext->KnockbackForce = OutImpact;	// 衝撃量
	//}
}