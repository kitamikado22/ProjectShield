// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_LockOn.h"
#include "Player/PSPlayerCharacter.h"
#include "Components/PSLockOnComponent.h"
#include "Utility/PSDebugMessageMacross.h"

UPSGA_LockOn::UPSGA_LockOn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UPSGA_LockOn::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APSPlayerCharacter* Player = Cast<APSPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (not Player)
	{
		DEBUG_MESSAGE_WARNING(TEXT("AvatarActor is not type of APSPlayerCharacter"));
		UE_LOG(LogTemp, Warning, TEXT("AvatarActor is not type of APSPlayerCharacter")); SET_LOG_PATH(Warning);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// ロックオンを有効・無効化する
	Player->GetLockOnComponent()->ToggleLockOn();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UPSGA_LockOn::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
