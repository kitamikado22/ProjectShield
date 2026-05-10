// Fill out your copyright notice in the Description page of Project Settings.


#include "PSStaminaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Utility/PSDebugMessageMacross.h"

UPSStaminaAttributeSet::UPSStaminaAttributeSet()
	: Stamina(1.0f), MaxStamina(1.0f)
{

}

bool UPSStaminaAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	return true;
}

void UPSStaminaAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UPSStaminaAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPSStaminaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPSStaminaAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetStaminaAttribute())
	{
		// スタミナが更新されたときのイベントを発行
		// ただし変化があったときだけ
		if (OldValue != NewValue)
		{
			OnStaminaUpdatedEvent->OnNext(NewValue);
		}
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		// スタミナの最大値が更新されたときのイベントを発行
		if (OldValue != NewValue)
		{
			OnMaxStaminaUpdatedEvent->OnNext(NewValue);
		}
	}
}

void UPSStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSStaminaAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSStaminaAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UPSStaminaAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSStaminaAttributeSet, Stamina, OldValue);
	OnStaminaUpdatedEvent->OnNext(GetStamina());
}

void UPSStaminaAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSStaminaAttributeSet, MaxStamina, OldValue);
	OnMaxStaminaUpdatedEvent->OnNext(GetMaxStamina());
}

void UPSStaminaAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}