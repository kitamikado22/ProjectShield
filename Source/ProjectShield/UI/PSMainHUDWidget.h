// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSStatusValueWidget.h"
#include "PSLockOnPointer.h"
#include "PSMainHUDWidget.generated.h"

/**
 * 画面上のHUDを表示するWidget
 */
UCLASS()
class PROJECTSHIELD_API UPSMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** 指定のアクタにロックオンポインタをマークする */
	void MarkLockOnPointerToTarget(const AActor* TargetActor);

	UPSStatusValueWidget* GetHealthBar() const { return HealthBar; }
	UPSStatusValueWidget* GetStaminaBar() const { return StaminaBar; }
	UPSStatusValueWidget* GetBreakBar() const { return BreakBar; }
	UPSLockOnPointer* GetLockOnPointer() const { return LockOnPointer; }
	
protected:

	/** 体力を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> HealthBar;

	/** スタミナを表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> StaminaBar;

	/** 体幹を表示するバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSStatusValueWidget> BreakBar;

	/** ロックオン時のポインター */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSLockOnPointer> LockOnPointer;
};
