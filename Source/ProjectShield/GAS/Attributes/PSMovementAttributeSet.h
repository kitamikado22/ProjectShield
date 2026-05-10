// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PSMovementAttributeSet.generated.h"

/**
 * 動きに関する属性セット
 */
UCLASS()
class PROJECTSHIELD_API UPSMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSMovementAttributeSet();

	/** 現在の移動速度 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "PS|Movement")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UPSMovementAttributeSet, MoveSpeed);

protected:

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION() void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
};
