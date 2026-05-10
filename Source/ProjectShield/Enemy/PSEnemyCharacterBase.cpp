// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PSEnemyCharacterBase.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Utility/PSMultiLoadManager.h"
#include "Components/CapsuleComponent.h"
#include "Player/PSPlayerController.h"
#include "Singleton/PSAudioManager.h"


APSEnemyCharacterBase::APSEnemyCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// 配置・スポーンされたときにAIが自動で所持するように設定
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APSEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// カメラとの衝突を無効化
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void APSEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APSEnemyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void APSEnemyCharacterBase::EngagePlayer(AActor* TargetPlayer)
{
	CurrentTargetPlayer = TargetPlayer;

	if (APawn* PlayerPawn = Cast<APawn>(TargetPlayer))
	{
		if (APSPlayerController* PlayerController = Cast<APSPlayerController>(PlayerPawn->GetController()))
		{
			// プレイヤーのHUDに敵の情報を表示する
			PlayerController->ShowEnemyHUD(this);
		}
	}

	// BGMを再生
	UPSAudioManager* AudioManager = UPSGameInstanceSubsystem::Get<UPSAudioManager>(this);
	AudioManager->PlayEnemyBGM(GetCharacterData()->SoftBGM);
}

void APSEnemyCharacterBase::DisengagePlayer()
{
	if (CurrentTargetPlayer.IsValid())
	{
		if (APawn* PlayerPawn = Cast<APawn>(CurrentTargetPlayer.Get()))
		{
			if (APSPlayerController* PlayerController = Cast<APSPlayerController>(PlayerPawn->GetController()))
			{
				// プレイヤーのHUDから敵の情報を非表示にする
				PlayerController->HideEnemyHUD();
			}
		}
	}

	// BGMをストップ
	UPSAudioManager* AudioManager = UPSGameInstanceSubsystem::Get<UPSAudioManager>(this);
	AudioManager->StopBGM();

	CurrentTargetPlayer.Reset();
}