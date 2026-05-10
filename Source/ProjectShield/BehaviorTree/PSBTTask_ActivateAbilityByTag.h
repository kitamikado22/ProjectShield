// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "PSBTTask_ActivateAbilityByTag.generated.h"

/**
 * Behavior Treeで指定したGameplay Tagを持つアビリティをアクティベートするタスクノード
 */
UCLASS()
class PROJECTSHIELD_API UPSBTTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UPSBTTask_ActivateAbilityByTag();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "PS|GAS")
	FGameplayTag AbilityTag;
};
