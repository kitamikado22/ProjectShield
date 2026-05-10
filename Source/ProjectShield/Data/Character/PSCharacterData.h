// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "Equipment/PSEquipmentType.h"
#include "PSCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FAttributeDefaultValue
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attribute")
	float Value;
};

/**
 * キャラクターを構成するデータを管理するクラス
 */
UCLASS()
class PROJECTSHIELD_API UPSCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/** キャラクターが持つアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilitys;

	/** キャラクターが持つ初期値のリスト */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Attribute")
	TArray<FAttributeDefaultValue> DefaultAttributes;

	/** 各部位に対する装備品のIDデータマップ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Equipment")
	TArray<FEquipmentIDAndSlot> DefaultEquipments;

	/** パッシブアビリティ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Ability")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilityClasses;

	/** 歩く速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Movement")
	float WalkSpeed = 150.f;
	/** 走る速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Movement")
	float RunSpeed = 600.f;
	/** スプリント速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Movement")
	float SprintSpeed = 900.f;
};
