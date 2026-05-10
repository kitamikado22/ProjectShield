// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Character/PSCharacterData.h"
#include "PSEnemyData.generated.h"

/**
 * 敵のデータクラス
 */
UCLASS()
class PROJECTSHIELD_API UPSEnemyData : public UPSCharacterData
{
	GENERATED_BODY()

public:

	/** 敵の名前 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Enemy")
	FText Name;

	/** 視認距離 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|AI")
	float SightRadius = 1500.f;

	/** 視認喪失距離 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|AI")
	float LoseSightRadius = 2000.f;

	/** 視野角 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|AI")
	float PeripheralVisionAngleDegrees = 60.f;

	/** 戦闘BGM */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Audio")
	TSoftObjectPtr<USoundBase> SoftBGM;
};
