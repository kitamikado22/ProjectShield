// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PSEnemyAIController.h"
#include "Utility/PSDebugMessageMacross.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/PSBlackbordKeyString.h"
#include "Enemy/PSEnemyCharacterBase.h"


APSEnemyAIController::APSEnemyAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;			// 視認距離
		SightConfig->LoseSightRadius = 2000.0f;		// 視認喪失距離
		SightConfig->PeripheralVisionAngleDegrees = 60.0f;	// 視野角
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		Perception->ConfigureSense(*SightConfig);
		Perception->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	SetPerceptionComponent(*Perception);

}

void APSEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (Perception)
	{
		Perception->OnTargetPerceptionUpdated.AddDynamic(this, &APSEnemyAIController::OnPerceptionUpdated);
	}
}

void APSEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	if (UBlackboardComponent* BBComp = GetBlackboardComponent())
	{
		FBlackboard::FKey TargetKeyID = BBComp->GetKeyID(PSBBKeys::TargetActor);
		BBComp->RegisterObserver(TargetKeyID, this, FOnBlackboardChangeNotification::CreateUObject(
			this, &APSEnemyAIController::OnTargetActorChanged));
	}

}

EBlackboardNotificationResult APSEnemyAIController::OnTargetActorChanged(const UBlackboardComponent& InBlackboard, FBlackboard::FKey KeyID)
{
	APSEnemyCharacterBase* Enemy = Cast<APSEnemyCharacterBase>(GetPawn());
	if (not Enemy)
	{
		DEBUG_MESSAGE_ERROR("Failed to cast Pawn to APSEnemyCharacterBase.");
		UE_LOG(LogTemp, Error, TEXT("Failed to cast Pawn to APSEnemyCharacterBase.")); SET_LOG_PATH(Error);
		return EBlackboardNotificationResult::RemoveObserver;
	}

	AActor* TargetActor = Cast<AActor>(InBlackboard.GetValueAsObject(PSBBKeys::TargetActor));

	// ターゲットが変更されたら
	if (TargetActor)
	{
		Enemy->EngagePlayer(TargetActor);
	}
	// ターゲットがいなくなったら
	else
	{
		Enemy->DisengagePlayer();
	}

	return EBlackboardNotificationResult::ContinueObserving;
}

void APSEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	// ブラックボード取得
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (not BB)
	{
		DEBUG_MESSAGE_ERROR("Failed to get Blackboard component.");
		UE_LOG(LogTemp, Error, TEXT("Failed to get Blackboard component.")); SET_LOG_PATH(Error);
		return;
	}

	// 視認した場合
	if (Stimulus.WasSuccessfullySensed())
	{
		DEBUG_MESSAGE(FString::Printf(TEXT("Enemy AI has spotted: %s"), *Actor->GetName()));
		UE_LOG(LogTemp, Log, TEXT("Enemy AI has spotted: %s"), *Actor->GetName());

		// 視認したキャラクターがプレイヤーであるか
		if (Actor->ActorHasTag(FName("Player")))
		{
			BB->SetValueAsObject(PSBBKeys::TargetActor, Actor);
			BB->SetValueAsBool(PSBBKeys::IsInCombat, true);
		}
	}

}