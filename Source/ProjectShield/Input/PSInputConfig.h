// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/** ゲーム内のアクションに対応する入力ID */
UENUM(BlueprintType)
enum class EPSAbilityInputID : uint8
{
	None,
	Jump,
	Sprint,
	Roll,
	LockOn,
	UseRightHand,
	UseRightHand_Second,
	UseLeftHand,
	UseLeftHand_Second,
	UseBothHands,
	UseBothHands_Second,
};

