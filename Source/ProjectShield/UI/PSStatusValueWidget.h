// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSStatusValueWidget.generated.h"

/**
 * 体力・スタミナのようなステータスの値を表示するウィジェット
 */
UCLASS()
class PROJECTSHIELD_API UPSStatusValueWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** バーの値を更新 */
	void UpdateValue(float NewValue);

	/** バーの最大値を更新 */
	void UpdateMaxValue(float NewValue);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;

	float CurrentValue = 1.0f;
	float CurrentMaxValue = 1.0f;

protected:

	/** バーの見た目を更新 */
	void UpdateProgressBar();
};
