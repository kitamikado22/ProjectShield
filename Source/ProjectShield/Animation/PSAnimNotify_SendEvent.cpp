// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAnimNotify_SendEvent.h"
#include "Utility/PSDebugMessageMacross.h"
#include "AbilitySystemComponent.h"
#include "Character/PSCharacterBase.h"

void UPSAnimNotify_SendEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (not EventTag.IsValid())
	{
		DEBUG_MESSAGE_WARNING(TEXT("EventTag is not valid"));
		UE_LOG(LogTemp, Warning, TEXT("EventTag is not valid")); SET_LOG_PATH(Warning);
		return;
	}

	if (AActor* Owner = MeshComp->GetOwner())
	{
		const bool bIsOwnerAuthority = Owner->HasAuthority();
		UE_LOG(LogTemp, Log, TEXT("PSAnimNotify_SendEvent fired. Owner: %s, Role: %d, Tag: %s"), *Owner->GetName(), (int32)Owner->GetLocalRole(), *EventTag.ToString()); SET_LOG_PATH(Log);

		if (APSCharacterBase* Character = Cast<APSCharacterBase>(Owner))
		{
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				ASC->HandleGameplayEvent(EventTag, &Payload);

				DEBUG_MESSAGE(FString::Printf(TEXT("Gameplay event sent: %s"), *EventTag.ToString()));
				UE_LOG(LogTemp, Log, TEXT("Gameplay event sent: %s"), *EventTag.ToString());
			}
		}
	}
}