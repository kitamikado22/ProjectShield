// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PSAnimNotifyState_SendEvent.h"
#include "Utility/PSDebugMessageMacross.h"
#include "AbilitySystemComponent.h"
#include "Character/PSCharacterBase.h"

void UPSAnimNotifyState_SendEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (APSCharacterBase* Character = Cast<APSCharacterBase>(Owner))
		{
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.EventTag = StartEventTag;
				Payload.Instigator = Owner;
				ASC->HandleGameplayEvent(StartEventTag, &Payload);

				UE_LOG(LogTemp, Log, TEXT("NotifyBegin: Sent event %s."), *StartEventTag.ToString());
			}
		}
	}
}

void UPSAnimNotifyState_SendEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (APSCharacterBase* Character = Cast<APSCharacterBase>(Owner))
		{
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.EventTag = EndEventTag;
				Payload.Instigator = Owner;
				ASC->HandleGameplayEvent(EndEventTag, &Payload);

				UE_LOG(LogTemp, Log, TEXT("NotifyEnd: Sent event %s."), *EndEventTag.ToString());
			}
		}
	}
}

void UPSAnimNotifyState_SendEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (APSCharacterBase* Character = Cast<APSCharacterBase>(Owner))
		{
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.EventTag = TickEventTag;
				Payload.Instigator = Owner;
				ASC->HandleGameplayEvent(TickEventTag, &Payload);

				UE_LOG(LogTemp, Log, TEXT("NotifyTick: Sent event %s."), *TickEventTag.ToString());
			}
		}
	}
}