// Fill out your copyright notice in the Description page of Project Settings.


#include "PSMovementAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPSMovementAttributeSet::UPSMovementAttributeSet()
	: MoveSpeed(600.0f)
{
}

void UPSMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// 移動速度が更新されたとき
	if (Attribute == GetMoveSpeedAttribute())
	{
		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (ACharacter* Character = Cast<ACharacter>(ASC->GetAvatarActor()))
			{
				Character->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
			}
		}
	}
}

void UPSMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPSMovementAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UPSMovementAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPSMovementAttributeSet, MoveSpeed, OldValue);
}