// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PSWeightAttributeSet.generated.h"

/**
 * 重量の属性値を管理する
 */
UCLASS()
class PROJECTSHIELD_API UPSWeightAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSWeightAttributeSet();

	/** 重量 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Weight", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Weight;
	ATTRIBUTE_ACCESSORS_BASIC(UPSWeightAttributeSet, Weight);

	/** 最大重量 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Weight", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxWeight;
	ATTRIBUTE_ACCESSORS_BASIC(UPSWeightAttributeSet, MaxWeight);


};
