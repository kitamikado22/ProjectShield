// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PSEquipmentType.generated.h"

/** 
 * 装備するスロット、装備する部位
 */
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    RightHand   UMETA(DisplayName = "Right Hand"),
    LeftHand    UMETA(DisplayName = "Left Hand"),
    Head        UMETA(DisplayName = "Head"),
    Body        UMETA(DisplayName = "Body"),
    Arm         UMETA(DisplayName = "Arm"),
    Leg         UMETA(DisplayName = "Leg")
};

/**
 * 装備スロットとそこに装備するアイテムのIDのペア
 */
USTRUCT(BlueprintType)
struct FEquipmentIDAndSlot
{
	GENERATED_BODY()

	/** 装備部位 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot Slot;

	/** 装備するアイテムのID */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;
};
