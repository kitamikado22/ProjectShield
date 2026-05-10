// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "PSEnemyAIController.generated.h"

/**
 * AIによって操作される敵キャラクターのコントローラ
 */
UCLASS()
class PROJECTSHIELD_API APSEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	APSEnemyAIController();

protected:

	/** AIの頭脳を担当するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|AI")
	UAIPerceptionComponent* Perception;

	/** AIの視覚設定 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|AI")
	UAISenseConfig_Sight* SightConfig;

	/** AIが使用するビヘイビアツリー */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|AI")
	UBehaviorTree* BehaviorTree;

protected:

	/** AIの視覚が更新されたときに呼び出される関数 */
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** ターゲットが変更されたときに呼ばれる */
	EBlackboardNotificationResult OnTargetActorChanged(const UBlackboardComponent& Blackboard, FBlackboard::FKey KeyID);

protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
