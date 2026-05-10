// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSEquipmentData.h"
#include "PSEquipmentOnHandsData.h"
#include "PSShieldData.generated.h"

/** 
* プレイヤーのステータスに依存した補正値
* 補正値 = 筋力 * 筋力係数 + 耐久力 * 耐久力係数etc...
* 生命力や持久力なども考慮した補正係数を追加することも可能
*/
USTRUCT(BlueprintType)
struct FPSCorrection
{
	GENERATED_BODY()

	/** 筋力係数 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Correction")
	float StrengthCoefficient;

	/** 耐久力係数 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Correction")
	float DurabilityCoefficient;
};

/** 盾などにつける補正値 */
USTRUCT(BlueprintType)
struct FPSShieldCorrection
{
	GENERATED_BODY()

	/** 防御力補正値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|ShieldCorrection")
	FPSCorrection DefenceCorrection;

	/** 緩衝力補正値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|ShieldCorrection")
	FPSCorrection PoiseCorrection;
};

/**
 * シールドを表すデータ
 */
UCLASS()
class PROJECTSHIELD_API UPSShieldData : public UPSEquipmentOnHandsData
{
	GENERATED_BODY()

public:

	/** 固有の防御力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float Defense;
	/** 固有の緩衝力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float Poise;

	/** ガードしたときの補正値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	FPSShieldCorrection GuardCorrection;
	/** 受け流し成功の補正値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	FPSShieldCorrection DeflectCorrection;
	/** 受け返し成功の補正値 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	FPSShieldCorrection ParryCorrection;

	/** ガードしたときのブレイクゲージへの伝送力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float GuardBreakRate;
	/** 受け流し成功のブレイクゲージへの伝送力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float DeflectBreakRate;
	/** 受け返し成功のブレイクゲージへの伝送力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float ParryBreakRate;

	/** ガード許容角度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float GuardAngle;

	/** 受け流し許容角度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float DeflectAngle;
	/** 受け流し許容時間 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float DeflectTiming;

	/** 受け返し許容角度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float ParryAngle;
	/** 受け返し許容時間 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Shield")
	float ParryTiming;
};
