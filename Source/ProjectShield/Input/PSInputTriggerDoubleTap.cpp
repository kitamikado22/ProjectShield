// Fill out your copyright notice in the Description page of Project Settings.


#include "PSInputTriggerDoubleTap.h"
#include "EnhancedPlayerInput.h"
#include "Utility/PSDebugMessageMacross.h"

ETriggerState UPSInputTriggerDoubleTap::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	const bool bCurrentlyActuated = IsActuated(ModifiedValue);

	ETriggerState Result = ETriggerState::None;

	// ダブルタップしているときはトリガー発動
	if (bIsDoubleTapping)
	{
		Result = ETriggerState::Triggered;
	}

	// 立ち上がり検出
	if (bCurrentlyActuated && not bWasActuated)
	{
		const float CurrentTime = PlayerInput->GetOuterAPlayerController()->GetWorld()->GetTimeSeconds();
		float Interval = CurrentTime - LastTapedTime;

		// チャタリング対策
		if (LastTapedTime >= 0.f && Interval <= DebounceThreshold)
		{
			// Result = ETriggerState::None;
		}
		else
		{
			// ダブルタップした瞬間
			if (Interval <= TapTimeWindow)
			{
				bIsDoubleTapping = true;
				LastTapedTime = -1.f;
			}
			// ダブルタップしていないときは最後に押した時間を更新
			else
			{
				LastTapedTime = CurrentTime;
			}
		}
	}
	// 立ち下がり検出
	else if (not bCurrentlyActuated && bWasActuated)
	{
		bIsDoubleTapping = false;
	}

	// 前フレームのボタンの状況を保存
	bWasActuated = bCurrentlyActuated;

	return Result;
}