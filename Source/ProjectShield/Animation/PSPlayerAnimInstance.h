// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PSDefaultAnimInstance.h"
#include "GameplayTagContainer.h"
#include "PSPlayerAnimInstance.generated.h"

/**
 * プレイヤー用のアニメーションインスタンス
 */
UCLASS()
class PROJECTSHIELD_API UPSPlayerAnimInstance : public UPSDefaultAnimInstance
{
	GENERATED_BODY()

protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	/** ダッシュしているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsSprinting;

	/** 入力によって動かそうとしているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsInputMoving;

	/** ダッシュジャンプをしているか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Movement")
	bool bIsSprintJumping;
	/** ダッシュジャンプのフラグを更新したか */
	bool bIsUpdateSprintJumping;

	/** ガードしているかどうか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Guard")
	bool bIsGuarding;

	/** 受け流しをしているかどうか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Guard")
	bool bIsDeflecting;

	/** 両手を使っているかどうか */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Combat")
	bool bIsUsingBothHanded;
};
