// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GAS/ExecutionCalculation/PSExecutionCalculation.h"
#include "PSExecCalc_Damage.generated.h"

/**
 * 敵からの攻撃を受けたときの計算を行うクラス
 */
UCLASS()
class PROJECTSHIELD_API UPSExecCalc_Damage : public UPSExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UPSExecCalc_Damage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
