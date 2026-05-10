// Fill out your copyright notice in the Description page of Project Settings.


#include "PSHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

UPSHealthAttributeSet::UPSHealthAttributeSet()
	: Health(1.0f), MaxHealth(1.0f)
{

}

bool UPSHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UPSHealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		/*DEBUG_MESSAGE(FString::Printf(TEXT("Damage applied: %f"), GetDamage()));
		UE_LOG(LogTemp, Log, TEXT("Damage applied: %f"), GetDamage());*/

		//if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Data.Target.GetAvatarActor()))
		//{
		//	if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		//	{
		//		// 無敵状態のタグが付いている場合はダメージを無効化
		//		if (ASC->HasMatchingGameplayTag(PSGameplayTags::State_Invincible))
		//		{
		//			DEBUG_MESSAGE(TEXT("This is invincible"));
		//			UE_LOG(LogTemp, Log, TEXT("This is invincible"));
		//			SetDamage(0.0f);
		//		}
		//	}
		//}

		// 体力をダメージで減らす
		SetHealth(GetHealth() - GetDamage());
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// 体力を回復して増やす
		SetHealth(GetHealth() + GetHealing());
		SetHealing(0.0f);
	}
}

void UPSHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPSHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPSHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Health changed: %f -> %f, Actor name is %s"), OldValue, NewValue, *GetOwningAbilitySystemComponent()->GetOwnerActor()->GetName()));
		UE_LOG(LogTemp, Log, TEXT("Health changed: %f -> %f, Actor name is %s"), OldValue, NewValue, *GetOwningAbilitySystemComponent()->GetOwnerActor()->GetName());

		// 体力が変化したことを通知
		if (OldValue != NewValue)
		{
			OnHealthUpdatedEvent->OnNext(NewValue);
		}
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// 体力の最大値が変化したこと通知
		if (OldValue != NewValue)
		{
			OnMaxHealthUpdatedEvent->OnNext(NewValue);
		}
	}
}

void UPSHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPSHealthAttributeSet, Health);
	DOREPLIFETIME(UPSHealthAttributeSet, MaxHealth);
}

void UPSHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSHealthAttributeSet, Health, OldValue);

	OnHealthUpdatedEvent->OnNext(GetHealth());
}

void UPSHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSHealthAttributeSet, MaxHealth, OldValue);

	OnMaxHealthUpdatedEvent->OnNext(GetMaxHealth());
}

void UPSHealthAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// 体力は負の値にならず、最大値を超えることはない
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// 体力の最大値は最低でも 1
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}