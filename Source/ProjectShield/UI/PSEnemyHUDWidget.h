// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/PSStatusValueWidget.h"
#include "Components/TextBlock.h"
#include "PSEnemyHUDWidget.generated.h"

/**
 * 戦闘中の敵の情報を表示するHUDWidget
 */
UCLASS()
class PROJECTSHIELD_API UPSEnemyHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UTextBlock* GetNameText() const { return NameText; }
	UPSStatusValueWidget* GetHealthBar() const { return HealthBar; }
	UPSStatusValueWidget* GetBreakBar() const { return BreakBar; }
	
protected:

	/** 敵の名前を表示するテキスト */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	/** 敵の体力を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> HealthBar;

	/** 敵の体幹を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> BreakBar;
};
