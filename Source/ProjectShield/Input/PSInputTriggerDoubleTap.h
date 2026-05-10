// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "PSInputTriggerDoubleTap.generated.h"

/**
 * ダブルタップ
 * 二回連続で同じキーを押したときに発動するトリガー
 */
UCLASS(NotBlueprintable, Config = Input, meta = (DisplayName = "Double Tap"))
class PROJECTSHIELD_API UPSInputTriggerDoubleTap : public UInputTrigger
{
	GENERATED_BODY()
	
protected:

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

public:

	/** ダブルタップとして判定する間の時間 */
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Trigger Settings")
	float TapTimeWindow = 0.4;

	/** チャタリング対策 */
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Trigger Settings")
	float DebounceThreshold = 0.05f;

private:

	float LastTapedTime = -1.f;
	bool bWasActuated = false;
	bool bIsDoubleTapping = false;
};
