// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PSDefaultAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHIELD_API UPSDefaultAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:

	/** キャラクターの速度 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	float Speed;

	/** キャラクターの視線に対する動く方向 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	float Direction;

	/** 加速度 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	float Acceleration;

	/** 動いているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsMoving;
	
	/** 加速しているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsAccelerating;

	/** 空中にいるか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsInAir;

	/** 落ちているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsFalling;

	/** 反転するか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsMirrored = false;

public:

	/** 反転するかを設定 */
	void SetIsMirrored(bool bNewIsMirrored) { bIsMirrored = bNewIsMirrored; }

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class APSCharacterBase> Character;

private:

	/** 1フレーム前の速度：加速度を求めるために使用 */
	float PreviousSpeed;

};
