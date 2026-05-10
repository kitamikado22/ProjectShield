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

};
