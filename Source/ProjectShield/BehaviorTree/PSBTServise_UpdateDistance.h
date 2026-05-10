// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "PSBTServise_UpdateDistance.generated.h"

/**
 * ターゲットとの距離を定期的に更新するサービスノード
 */
UCLASS()
class PROJECTSHIELD_API UPSBTServise_UpdateDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UPSBTServise_UpdateDistance();
	
protected:

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
