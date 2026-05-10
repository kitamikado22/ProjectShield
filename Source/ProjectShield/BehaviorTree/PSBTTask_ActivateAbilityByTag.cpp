// Fill out your copyright notice in the Description page of Project Settings.


#include "PSBTTask_ActivateAbilityByTag.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UPSBTTask_ActivateAbilityByTag::UPSBTTask_ActivateAbilityByTag()
{
	NodeName = TEXT("Activate Ability By Tag");
}

EBTNodeResult::Type UPSBTTask_ActivateAbilityByTag::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	if (not AbilityTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (not AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (not Pawn)
	{
		return EBTNodeResult::Failed;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn);
	if (not AbilitySystemInterface)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (not AbilitySystemComponent)
	{
		return EBTNodeResult::Failed;
	}
	
	// 指定したタグを持つアビリティの実行を試みる
	if (AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag)))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;	
}