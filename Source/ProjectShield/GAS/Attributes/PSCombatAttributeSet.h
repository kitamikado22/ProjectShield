// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PSCombatAttributeSet.generated.h"

/**
 * 戦闘に関するパラメータ
 */
UCLASS()
class PROJECTSHIELD_API UPSCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSCombatAttributeSet();

	/** 攻撃力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Combat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS_BASIC(UPSCombatAttributeSet, Attack);

	/** 衝撃力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Combat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Impact;
	ATTRIBUTE_ACCESSORS_BASIC(UPSCombatAttributeSet, Impact);

	/** 防御力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Combat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS_BASIC(UPSCombatAttributeSet, Defense);

	/** 緩衝力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Combat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Poise;
	ATTRIBUTE_ACCESSORS_BASIC(UPSCombatAttributeSet, Poise);

};
