// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGameplayAbility.h"
#include "Utility/PSDebugMessageMacross.h"
#include "AbilitySystemComponent.h"

UPSGameplayAbility::UPSGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

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

FActiveGameplayEffectHandle UPSGameplayAbility::AddDynamicTagToOwner(FGameplayTag Tag)
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

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DynamicTagEffectClass);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->DynamicGrantedTags.AddTag(Tag);

		FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle);
		
		if (EffectHandle.IsValid())
		{
			ActiveDynamicTagHandles.Add(Tag, EffectHandle);
			return EffectHandle;
		}
	}

	return FActiveGameplayEffectHandle();
}

void UPSGameplayAbility::RemoveDynamicTagFromOwner(FGameplayTag Tag)
{
	if (not ActiveDynamicTagHandles.Contains(Tag)) 
	{
		DEBUG_MESSAGE_WARNING(TEXT("Tag does not exist on owner, skipping RemoveDynamicTagFromOwner"));
		UE_LOG(LogTemp, Warning, TEXT("Tag does not exist on owner, skipping RemoveDynamicTagFromOwner")); SET_LOG_PATH(Warning);
		return;
	}

	if (FActiveGameplayEffectHandle* HandlePtr = ActiveDynamicTagHandles.Find(Tag))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveActiveGameplayEffect(*HandlePtr);
		}
	}

	ActiveDynamicTagHandles.Remove(Tag);
}

void UPSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// アビリティが終了するときに、付与したタグをすべて削除する
	for (const auto& Pair : ActiveDynamicTagHandles)
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveActiveGameplayEffect(Pair.Value);
		}
	}
	ActiveDynamicTagHandles.Empty();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}