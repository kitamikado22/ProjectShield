// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Character/PSCharacterData.h"
#include "PSPlayerData.generated.h"

/**
 * プレイヤーのデータクラス
 */
UCLASS()
class PROJECTSHIELD_API UPSPlayerData : public UPSCharacterData
{
	GENERATED_BODY()
	
public:

	/** ダッシュアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TSubclassOf<class UPSGA_Sprint> SprintAbilityClass;

	/** ジャンプアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TSubclassOf<class UPSGA_Jump> JumpAbilityClass;

	/** ローリングアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TSubclassOf<class UPSGA_Roll> RollAbilityClass;

	/** ロックオンアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TSubclassOf<class UPSGA_LockOn> LockOnAbilityClass;


	/** カメラとプレイヤーの距離 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Camera")
	float TargetArmLength = 300.f;

	/** 通常状態のカメラのオフセット */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Camera")
	FVector NormalCameraOffset = FVector(0.f, 0.f, 70.f);

	/** ロックオン時のカメラのオフセット */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Camera")
	FVector LockOnCameraOffset = FVector(0.f, 80.f, 20.f);
};
