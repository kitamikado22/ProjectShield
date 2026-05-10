// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GAS/PSGameplayEffectContext.h"
#include "PSAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHIELD_API UPSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override
	{
		return new FPSGameplayEffectContext();
	}
};
