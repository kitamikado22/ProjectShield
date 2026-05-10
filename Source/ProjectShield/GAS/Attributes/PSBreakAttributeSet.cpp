// Fill out your copyright notice in the Description page of Project Settings.


#include "PSBreakAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Utility/PSDebugMessageMacross.h"

UPSBreakAttributeSet::UPSBreakAttributeSet()
	: Break(0.0f), MaxBreak(1.0f)
{
}

bool UPSBreakAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	return true;
}

void UPSBreakAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetRecoverAttribute())
	{
		// ブレイクゲージを回復
		SetBreak(GetBreak() - GetRecover());
		SetRecover(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetWorsenAttribute())
	{
		// ブレイクゲージを増加
		SetBreak(GetBreak() + GetWorsen());
		OnWorsonUpdateEvent->OnNext(Data);
		SetWorsen(0.0f);
	}
}

void UPSBreakAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPSBreakAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPSBreakAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetBreakAttribute())
	{
		// 体幹が更新されたときのイベントを発行
		if (OldValue != NewValue)
		{
			OnBreakUpdatedEvent->OnNext(NewValue);
		}
	}
	else if (Attribute == GetMaxBreakAttribute())
	{
		// 体幹の最大値が更新されたときのイベントを発行
		if (OldValue != NewValue)
		{
			OnMaxBreakUpdatedEvent->OnNext(NewValue);
		}
	}
}

void UPSBreakAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSBreakAttributeSet, Break, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSBreakAttributeSet, MaxBreak, COND_None, REPNOTIFY_Always);
}

void UPSBreakAttributeSet::OnRep_Break(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSBreakAttributeSet, Break, OldValue);
	OnBreakUpdatedEvent->OnNext(GetBreak());
}

void UPSBreakAttributeSet::OnRep_MaxBreak(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSBreakAttributeSet, MaxBreak, OldValue);
	OnMaxBreakUpdatedEvent->OnNext(GetMaxBreak());
}

void UPSBreakAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetBreakAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxBreak());
	}
	else if (Attribute == GetMaxBreakAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}