// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PSStatusAttributeSet.generated.h"

/**
 * プレイヤーの成長要素であるステータス
 */
UCLASS()
class PROJECTSHIELD_API UPSStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSStatusAttributeSet();

	/** レベル */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Status", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStatusAttributeSet, Level);

	/** 生命力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Status", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStatusAttributeSet, Vitality);

	/** 持久力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Status", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStatusAttributeSet, Endurance);

	/** 筋力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Status", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStatusAttributeSet, Strength);

	/** 耐久力 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Status", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Durability;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStatusAttributeSet, Durability);

};
