// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_MeleeAttack.h"
#include "Character/PSCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Components/PSAttackComponent.h"


void UPSGA_MeleeAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData 
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPSGA_MeleeAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPSGA_MeleeAttack::OnWeaponHit(const FHitResult& HitResult)
{
	Super::OnWeaponHit(HitResult);
}

void UPSGA_MeleeAttack::StartAttackTrace(FGameplayEventData Payload)
{
	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("StartAttack: AvatarActor is not a valid character"));
		UE_LOG(LogTemp, Error, TEXT("StartAttack: AvatarActor is not a valid character")); SET_LOG_PATH(Error);
		return;
	}

	UPSAttackComponent* AttackComponent = Character->GetAttackComponent();
	USkeletalMeshComponent* Mesh = Character->GetMesh();

	// 攻撃トレース開始
	AttackTraceID = AttackComponent->StartAttackTrace(Mesh, RootSocket, TipSocket, AttackRadius, [this](const FHitResult& Hit)
		{
			OnWeaponHit(Hit);
		});
}

void UPSGA_MeleeAttack::EndAttackTrace(FGameplayEventData Payload)
{
	APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
	if (not Character)
	{
		DEBUG_MESSAGE_ERROR(TEXT("EndAttack: AvatarActor is not a valid character"));
		UE_LOG(LogTemp, Error, TEXT("EndAttack: AvatarActor is not a valid character")); SET_LOG_PATH(Error);
		return;
	}

	UPSAttackComponent* AttackComponent = Character->GetAttackComponent();

	// 攻撃トレース終了
	AttackComponent->EndAttackTrace(AttackTraceID);
}
