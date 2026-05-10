// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAbilitySystemComponent.h"
#include "Utility/PSDebugMessageMacross.h"

UPSAbilitySystemComponent::UPSAbilitySystemComponent()
{
	// タグ付与用のGEをロード
	static ConstructorHelpers::FClassFinder<UGameplayEffect> TagEffectFinder(TEXT("/Game/Blueprints/GAS/GameplayEffects/GE_DynamicTag.GE_DynamicTag_C"));
	if (TagEffectFinder.Succeeded())
	{
		DynamicTagEffectClass = TagEffectFinder.Class;
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to find DynamicTag GameplayEffect class"));
		UE_LOG(LogTemp, Error, TEXT("Failed to find DynamicTag GameplayEffect class")); SET_LOG_PATH(Error);
	}
}

FActiveGameplayEffectHandle UPSAbilitySystemComponent::AddDynamicTagToOwner(FGameplayTag Tag)
{
	if (not DynamicTagEffectClass)
	{
		DEBUG_MESSAGE_ERROR(TEXT("DynamicTagEffectClass is not set"));
		UE_LOG(LogTemp, Error, TEXT("DynamicTagEffectClass is not set")); SET_LOG_PATH(Error);
		return FActiveGameplayEffectHandle();
	}
	if (not Tag.IsValid()) 
	{
		DEBUG_MESSAGE_ERROR(TEXT("Invalid tag provided to AddDynamicTagToOwner"));
		UE_LOG(LogTemp, Error, TEXT("Invalid tag provided to AddDynamicTagToOwner")); SET_LOG_PATH(Error);
		return FActiveGameplayEffectHandle();
	}
	// タグの重複防止
	if (ActiveDynamicTagHandles.Contains(Tag)) 
	{
		DEBUG_MESSAGE_WARNING(TEXT("Tag already exists on owner, skipping AddDynamicTagToOwner"));
		UE_LOG(LogTemp, Warning, TEXT("Tag already exists on owner, skipping AddDynamicTagToOwner")); SET_LOG_PATH(Warning);
		return ActiveDynamicTagHandles[Tag];
	}

	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagEffectClass, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->DynamicGrantedTags.AddTag(Tag);
		FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
		if (EffectHandle.IsValid())
		{
			ActiveDynamicTagHandles.Add(Tag, EffectHandle);
			return EffectHandle;
		}
		else
		{
			DEBUG_MESSAGE_ERROR(TEXT("Failed to apply DynamicTag GameplayEffect to owner"));
			UE_LOG(LogTemp, Error, TEXT("Failed to apply DynamicTag GameplayEffect to owner")); SET_LOG_PATH(Error);
			return FActiveGameplayEffectHandle();
		}
	}
	else
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to create spec handle for DynamicTag GameplayEffect"));
		UE_LOG(LogTemp, Error, TEXT("Failed to create spec handle for DynamicTag GameplayEffect")); SET_LOG_PATH(Error);
		return FActiveGameplayEffectHandle();
	}
}

void UPSAbilitySystemComponent::RemoveDynamicTagFromOwner(FGameplayTag Tag)
{
	if (not ActiveDynamicTagHandles.Contains(Tag)) 
	{
		DEBUG_MESSAGE_WARNING(TEXT("Tag does not exist on owner, skipping RemoveDynamicTagFromOwner"));
		UE_LOG(LogTemp, Warning, TEXT("Tag does not exist on owner, skipping RemoveDynamicTagFromOwner")); SET_LOG_PATH(Warning);
		return;
	}

	if (FActiveGameplayEffectHandle* HandlePtr = ActiveDynamicTagHandles.Find(Tag))
	{
		RemoveActiveGameplayEffect(*HandlePtr);
	}

	ActiveDynamicTagHandles.Remove(Tag);
}