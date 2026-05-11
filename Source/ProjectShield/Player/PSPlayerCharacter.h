// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character\PSCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "Utility/Observer/PSSubject.h"
#include "Input/PSInputConfig.h"
#include "Data/Character/PSPlayerData.h"
#include "PSPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPSLockOnComponent;
class UInputAction;
struct FInputActionValue;

/**
 * 操作するプレイヤーのキャラクター
 */
UCLASS(Blueprintable)
class PROJECTSHIELD_API APSPlayerCharacter : public APSCharacterBase
{
	GENERATED_BODY()

public:

	APSPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	/** 追跡するカメラの挙動を制御するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	/** プレイヤーを追跡するカメラ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Components")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** ロックオンのロジックを担当するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Components")
	TObjectPtr<UPSLockOnComponent> LockOnComponent;

protected:

	/** プレイヤーを移動させる入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> MoveAction;

	/** カメラで見渡す入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> LookAction;

	/** 右手を使用する入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseRightHandAction;
	/** 右手を使用する入力アクション2 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseRightHandAction_Second;

	/** 左手を使用する入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseLeftHandAction;
	/** 左手を使用する入力アクション2 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseLeftHandAction_Second;

	/** 両手を使用する入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseBothHandsAction;
	/** 両手を使用する入力アクション2 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> UseBothHandsAction_Second;

	/** ジャンプ入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> JumpAction;

	/** ダッシュ入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> SprintAction;

	/** ローリング入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> RollAction;

	/** ロックオン入力アクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|Input")
	TObjectPtr<UInputAction> LockOnAction;

public:

	/** ロックオンコンポーネント取得 */
	UPSLockOnComponent* GetLockOnComponent() const { return LockOnComponent; }

	/** プレイヤーのデータを取得 */
	virtual UPSPlayerData* GetCharacterData() const override { return Cast<UPSPlayerData>(CharacterData); }

	/** 現在の移動入力値取得 */
	FVector2D GetMoveInput() const { return MoveInput; }

public:
	/** 動いたときのイベント取得 */
	TSharedRef<IPSObservable<FVector2D>> GetOnMoveEvent() const { return OnMoveEvent; }
protected:
	/** 動いたときのイベント */
	TSharedRef<TPSSubject<FVector2D>> OnMoveEvent = TPSSubject<FVector2D>::Create();

protected:
	/** 現在の移動入力値 */
	FVector2D MoveInput;

protected:

	/** 入力ベクトルに対してキャラクターを動かす */
	void Move(const FInputActionValue& Value);
	/** 入力ベクトルに対してカメラを動かす */
	void Look(const FInputActionValue& Value);
	/** 右手の装備品を使用する1 */
	void OnPressedUseRightHand();
	/** 右手の装備品の使用をやめる1 */
	void OnReleasedUseRightHand();
	/** 右手の装備品を使用する2 */
	void OnPressedUseRightHand_Second();
	/** 右手の装備品の使用をやめる2 */
	void OnReleasedUseRightHand_Second();
	/** 左手の装備品を使用する1 */
	void OnPressedUseLeftHand();
	/** 左手の装備品の使用をやめる1*/
	void OnReleasedUseLeftHand();
	/** 左手の装備品を使用する2 */
	void OnPressedUseLeftHand_Second();
	/** 左手の装備品の使用をやめる2 */
	void OnReleasedUseLeftHand_Second();
	/** 両手で装備品を使用する1 */
	void OnPressedUseBothHands();
	/** 両手で装備品の使用をやめる1 */
	void OnReleasedUseBothHands();
	/** 両手で装備品を使用する2 */
	void OnPressedUseBothHands_Second();
	/** 両手で装備品の使用をやめる2 */
	void OnReleasedUseBothHands_Second();
	/** ジャンプ入力 */
	void OnPressedJump();
	/** スプリント入力 */
	void OnPressedSprint();
	/** ローリング入力 */
	void OnPressedRoll();
	/** ロックオン入力 */
	void OnPressedLockOn();

	/** 指定のInputIDが押されたことをASC側に伝える */
	void AbilityInputPressed(EPSAbilityInputID InputID);

	/** 指定のInputIDが離されたことをASC側に伝える */
	void AbiilityInputReleased(EPSAbilityInputID InputID);

protected:

	/** ブレイク値が悪化したときに呼ばれる */
	void OnWorsonUpdate(const FGameplayEffectModCallbackData& Data);

	/** 入力無効タグが追加・削除されたときに呼ばれる */
	void OnDisableInputTagChanged(const FGameplayTag Tag, int32 NewCount);

	/** カメラの設定などを状況によって更新 */
	void UpdateCamera(float DeltaTime);

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** コントローラがキャラクターを所持した瞬間に呼ばれる */
	virtual void PossessedBy(AController* NewController) override;

	/** 入力アクションを初期化 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
