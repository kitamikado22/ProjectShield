// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PSLockOnPointer.generated.h"

/**
 * ターゲットロックオンしたときに画面に出るポインター
 */
UCLASS()
class PROJECTSHIELD_API UPSLockOnPointer : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
public:

	/** 非表示 */
	void Hide();
	/** 表示 */
	void Show();

protected:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PulseAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PointerImage;
};
