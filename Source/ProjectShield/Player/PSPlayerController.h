// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Utility/Observer/PSSubject.h"
#include "PSPlayerController.generated.h"

class UInputMappingContext;
class UPSMainHUDWidget;
class UPSEnemyHUDWidget;
class APSEnemyCharacterBase;

/**
 * プレイヤーのキャラクターを操作するコントローラ
 */
UCLASS()
class PROJECTSHIELD_API APSPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	/** プレイヤーのHUDウィジェットを取得 */
	UPSMainHUDWidget* GetMainHUDWidget() const { return MainHUDWidget; }

public:

	/** 移動に関係する入力マップを取得 */
	UInputMappingContext* GetIMC_Movement() const { return IMC_Movement; }
	/** 戦闘に関係する入力マップを取得 */
	UInputMappingContext* GetIMC_Combat() const { return IMC_Combat; }

protected:

	/** デフォルトの入力マップ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputMappingContext> IMC_Default;

	/** 移動に関係する入力マップ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputMappingContext> IMC_Movement;

	/** 戦闘に関係する入力マップ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputMappingContext> IMC_Combat;

	/** 入力マップを初期化 */
	virtual void SetupInputComponent() override;

protected:

	/** 敵のHUDウィジェットクラス */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|UI")
	TSubclassOf<UPSEnemyHUDWidget> EnemyHUDWidgetClass;

	/** プレイヤーのHUDウィジェットクラス */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|UI")
	TSubclassOf<UPSMainHUDWidget> MainHUDWidgetClass;

	/** プレイヤーのHUDウィジェットクラスのインスタンス */
	UPROPERTY()
	TObjectPtr<UPSMainHUDWidget> MainHUDWidget;

	/** 敵のHUDウィジェットクラスのインスタンス */
	UPROPERTY()
	TObjectPtr<UPSEnemyHUDWidget> EnemyHUDWidget;

protected:

	/** 敵の体力変更イベント解除用 */
	TSharedPtr<IPSDisposable> EnemyHealthUpdateDisposable;
	/** 敵の最大体力変更イベント解除用 */
	TSharedPtr<IPSDisposable> EnemyMaxHealthUpdateDisposable;
	/** 敵のブレイク変更イベント解除用 */
	TSharedPtr<IPSDisposable> EnemyBreakUpdateDisposable;
	/** 敵の最大ブレイク変更イベント解除用 */
	TSharedPtr<IPSDisposable> EnemyMaxBreakUpdateDisposable;

public:

	/** 敵のHUDウィジェットを表示する */
	void ShowEnemyHUD(APSEnemyCharacterBase* Enemy);
	/** 敵のHUDウィジェットを非表示にする */
	void HideEnemyHUD();

protected:

	/** ゲーム開始時に呼び出される関数 */
	virtual void BeginPlay() override;
};
