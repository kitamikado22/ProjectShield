// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GAS/ExecutionCalculation/PSExecutionCalculation.h"
#include "PSExecCalc_DamageFeedback.generated.h"

/**
 * ダメージを与えた際にフィードバック的にオーナに影響を及ぼすGameplayEffectのExecutionCalculation
 */
UCLASS()
class PROJECTSHIELD_API UPSExecCalc_DamageFeedback : public UPSExecutionCalculation
{
	GENERATED_BODY()

public:

	UPSExecCalc_DamageFeedback();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
