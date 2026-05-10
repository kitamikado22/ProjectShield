// Fill out your copyright notice in the Description page of Project Settings.


#include "PSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PSMainHUDWidget.h"
#include "UI/PSEnemyHUDWidget.h"
#include "Enemy/PSEnemyCharacterBase.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/Attributes/PSHealthAttributeSet.h"
#include "GAS/Attributes/PSBreakAttributeSet.h"
#include "Data/Character/PSEnemyData.h"


void APSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_Default, 0);
		Subsystem->AddMappingContext(IMC_Movement, 10);
		Subsystem->AddMappingContext(IMC_Combat, 20);
	}
}

void APSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MainHUDWidgetClass)
	{
		MainHUDWidget = CreateWidget<UPSMainHUDWidget>(this, MainHUDWidgetClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
	}
}

void APSPlayerController::ShowEnemyHUD(APSEnemyCharacterBase* Enemy)
{
	if (not EnemyHUDWidgetClass)
	{
		DEBUG_MESSAGE_ERROR(TEXT("EnemyHUDWidgetClass is not set in PlayerController"));
		UE_LOG(LogTemp, Error, TEXT("EnemyHUDWidgetClass is not set in PlayerController")); SET_LOG_PATH(Error);
		return;
	}

	// すでに敵のHUDが表示されている場合
	if (EnemyHUDWidget)
	{
		DEBUG_MESSAGE_WARNING(TEXT("Enemy HUD is already displayed. Removing existing one before showing new one."));
		UE_LOG(LogTemp, Warning, TEXT("Enemy HUD is already displayed. Removing existing one before showing new one.")); SET_LOG_PATH(Warning);
		return;
	}

	// 敵のHUDを表示する
	EnemyHUDWidget = CreateWidget<UPSEnemyHUDWidget>(this, EnemyHUDWidgetClass);
	EnemyHUDWidget->AddToViewport();

	UPSEnemyData* EnemyData = Enemy->GetCharacterData();
	if (not EnemyData)
	{
		DEBUG_MESSAGE_ERROR(TEXT("CharacterData is not valid EnemyData"));
		UE_LOG(LogTemp, Error, TEXT("CharacterData is not valid EnemyData")); SET_LOG_PATH(Error);
		return;
	}

	// 敵の名前をセット
	EnemyHUDWidget->GetNameText()->SetText(EnemyData->Name);

	// 敵の体力とブレイクの更新イベントに購読する
	EnemyHealthUpdateDisposable = Enemy->GetHealthAttributeSet()->GetOnHealthUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			EnemyHUDWidget->GetHealthBar()->UpdateValue(NewValue);
		});
	EnemyMaxHealthUpdateDisposable = Enemy->GetHealthAttributeSet()->GetOnMaxHealthUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			EnemyHUDWidget->GetHealthBar()->UpdateMaxValue(NewValue);
		});
	EnemyBreakUpdateDisposable = Enemy->GetBreakAttributeSet()->GetOnBreakUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			EnemyHUDWidget->GetBreakBar()->UpdateValue(NewValue);
		});
	EnemyMaxBreakUpdateDisposable = Enemy->GetBreakAttributeSet()->GetOnMaxBreakUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			EnemyHUDWidget->GetBreakBar()->UpdateMaxValue(NewValue);
		});

	// UIに初期値を設定
	EnemyHUDWidget->GetHealthBar()->UpdateValue(Enemy->GetHealthAttributeSet()->GetHealth());
	EnemyHUDWidget->GetHealthBar()->UpdateMaxValue(Enemy->GetHealthAttributeSet()->GetMaxHealth());
	EnemyHUDWidget->GetBreakBar()->UpdateValue(Enemy->GetBreakAttributeSet()->GetBreak());
	EnemyHUDWidget->GetBreakBar()->UpdateMaxValue(Enemy->GetBreakAttributeSet()->GetMaxBreak());

}

void APSPlayerController::HideEnemyHUD()
{
	if (EnemyHUDWidget && EnemyHUDWidget->IsInViewport())
	{
		EnemyHUDWidget->RemoveFromParent();
		EnemyHUDWidget = nullptr;

		if (EnemyHealthUpdateDisposable.IsValid())
		{
			EnemyHealthUpdateDisposable->Dispose();
			EnemyHealthUpdateDisposable.Reset();
		}
		if (EnemyMaxHealthUpdateDisposable.IsValid())
		{
			EnemyMaxHealthUpdateDisposable->Dispose();
			EnemyMaxHealthUpdateDisposable.Reset();
		}
		if (EnemyBreakUpdateDisposable.IsValid())
		{
			EnemyBreakUpdateDisposable->Dispose();
			EnemyBreakUpdateDisposable.Reset();
		}
		if (EnemyMaxBreakUpdateDisposable.IsValid())
		{
			EnemyMaxBreakUpdateDisposable->Dispose();
			EnemyMaxBreakUpdateDisposable.Reset();
		}
	}
}