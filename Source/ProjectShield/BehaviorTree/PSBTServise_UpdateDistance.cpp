// Fill out your copyright notice in the Description page of Project Settings.


#include "PSBTServise_UpdateDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PSBlackbordKeyString.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Character/PSCharacterBase.h"
#include "Data/Character/PSEnemyData.h"

UPSBTServise_UpdateDistance::UPSBTServise_UpdateDistance()
{
	NodeName = TEXT("Update Distance To Target");

	bNotifyTick = true;
	Interval = 0.5f;
}

void UPSBTServise_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (not ControlledPawn)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PSBBKeys::TargetActor));
	if (not TargetActor)
	{
		return;
	}

	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(PSBBKeys::DistanceToTarget, Distance);

	APSCharacterBase* EnemyCharacter = Cast<APSCharacterBase>(ControlledPawn);
	if (not EnemyCharacter)
	{
		DEBUG_MESSAGE_ERROR(TEXT("ControlledPawn is not a valid EnemyCharacter"));
		UE_LOG(LogTemp, Error, TEXT("ControlledPawn is not a valid EnemyCharacter")); SET_LOG_PATH(Error);
		return;
	}

	UPSEnemyData* EnemyData = Cast<UPSEnemyData>(EnemyCharacter->GetCharacterData());
	if (not EnemyData)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Used CharacterData is not EnemyData"));
		UE_LOG(LogTemp, Error, TEXT("Used CharacterData is not EnemyData")); SET_LOG_PATH(Error);
		return;
	}

	// ターゲットとの距離が遠すぎると見失う
	if (Distance > EnemyData->LoseSightRadius)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(PSBBKeys::TargetActor);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(PSBBKeys::IsInCombat, false);
	}
}