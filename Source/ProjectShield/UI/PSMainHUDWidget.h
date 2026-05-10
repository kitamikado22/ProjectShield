// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSStatusValueWidget.h"
#include "PSMainHUDWidget.generated.h"

/**
 * 画面上のHUDを表示するWidget
 */
UCLASS()
class PROJECTSHIELD_API UPSMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPSStatusValueWidget* GetHealthBar() const { return HealthBar; }
	UPSStatusValueWidget* GetStaminaBar() const { return StaminaBar; }
	UPSStatusValueWidget* GetBreakBar() const { return BreakBar; }

protected:

	/** 体力を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> HealthBar;

	/** スタミナを表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> StaminaBar;

	/** 体幹を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> BreakBar;
};
