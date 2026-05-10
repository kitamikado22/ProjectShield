// Fill out your copyright notice in the Description page of Project Settings.


#include "PSDefenseComponent.h"
#include "Data/Item/PSShieldData.h"
#include "Character/PSCharacterBase.h"
#include "Equipment/PSEquipmentComponent.h"
#include "GAS/PSGameplayTags.h"

/** 
 * 許容角度内にあるか確認
 * @param OwnerForward オーナーの前方ベクトル
 * @param AttackDir 攻撃の方向ベクトル
 * @param MaxAngleDegrees 許容角度、単位は度
 * @return 許容角度内にある場合はtrue、そうでない場合はfalse
 */
static bool IsWithinAngle(const FVector& OwnerForward, const FVector& AttackDir, float MaxAngleDegree)
{
	// 攻撃方向ベクトルを反転させ、オーナーの前方ベクトルと比較しやすくする
	// 例えば、正面から攻撃を受ける場合、ベクトルは同じ向きになり角度は0度になる
	FVector InvAttackDir = -AttackDir;

	float Dot = FVector::DotProduct(OwnerForward.GetSafeNormal(), InvAttackDir);

	// 許容角度はプレイヤーの角度を基準に左右対称であるため、最大角度の半分を計算して使用する
	float HalfAngleRad = FMath::DegreesToRadians(MaxAngleDegree * 0.5f);
	// ドット積の値が許容角度のコサイン値以上であれば、攻撃は許容角度内にあると判断できる
	float CosThreshold = FMath::Cos(HalfAngleRad);

	return Dot >= CosThreshold;
}

EPSDefenseType UPSDefenseComponent::DecideDefenseType(const FHitResult& HitResult)
{
	APSCharacterBase* Character = Cast<APSCharacterBase>(GetOwner());

	// 現在使っている盾のデータを取得
	UPSShieldData* ShieldData = Cast<UPSShieldData>(Character->GetEquipmentComponent()->GetUsedEquipmentData());
	
	// 盾を装備していないまたは使っていない場合
	if (not ShieldData)
	{	
		DEBUG_MESSAGE(TEXT("Not equipped with or using a shield"));
		UE_LOG(LogTemp, Log, TEXT("Not equipped with or using a shield"));
		return EPSDefenseType::None;
	}

	// オーナのタグを取得
	FGameplayTagContainer OwnerTags = Character->GetAbilitySystemComponent()->GetOwnedGameplayTags();

	FVector AttackDirection = HitResult.ImpactPoint * -1.f;

	// 盾で受け返している場合
	if (OwnerTags.HasTag(PSGameplayTags::State_Parry))
	{
		// 許容角度であるか確認
		if (IsWithinAngle(Character->GetActorForwardVector(), AttackDirection, ShieldData->ParryAngle))
		{
			DEBUG_MESSAGE(TEXT("Defense type is Parry"));
			UE_LOG(LogTemp, Log, TEXT("Defense type is Parry"));
			return EPSDefenseType::Parry;
		}
	}
	// 盾で受け流している場合
	else if (OwnerTags.HasTag(PSGameplayTags::State_Deflect))
	{
		// 許容角度であるか確認
		if (IsWithinAngle(Character->GetActorForwardVector(), AttackDirection, ShieldData->DeflectAngle))
		{
			DEBUG_MESSAGE(TEXT("Defense type is Deflect"));
			UE_LOG(LogTemp, Log, TEXT("Defense type is Deflect"));
			return EPSDefenseType::Deflect;
		}
	}
	// 盾で受け止めている場合
	else if (OwnerTags.HasTag(PSGameplayTags::State_Guard))
	{
		// 許容角度であるか確認
		if (IsWithinAngle(Character->GetActorForwardVector(), AttackDirection, ShieldData->GuardAngle))
		{
			DEBUG_MESSAGE(TEXT("Defense type is Guard"));
			UE_LOG(LogTemp, Log, TEXT("Defense type is Guard"));
			return EPSDefenseType::Guard;
		}
	}

	DEBUG_MESSAGE(TEXT("Failed to defend"));
	UE_LOG(LogTemp, Log, TEXT("Failed to defend"));

	// 防御行動に失敗している場合など
	return EPSDefenseType::None;
}