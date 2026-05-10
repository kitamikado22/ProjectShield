// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "Utility/Observer/PSSubject.h"
#include "PSCharacterBase.generated.h"

class UPSEquipmentComponent;
class UPSDefenseComponent;
class UPSAttackComponent;
class UPSAbilitySystemComponent;
class UPSStaminaAttributeSet;
class UPSHealthAttributeSet;
class UPSBreakAttributeSet;
class UPSCombatAttributeSet;
class UPSStatusAttributeSet;
class UPSMovementAttributeSet;
class UPSWeightAttributeSet;
class UGameplayAbility;
class UAIPerceptionStimuliSourceComponent;

/**
 * プレイヤー、敵など全てのキャラクターの基底クラス
 */
UCLASS(Abstract)
class PROJECTSHIELD_API APSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APSCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:

	/** 装備を管理するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Equipment")
	TObjectPtr<UPSEquipmentComponent> EquipmentComponent;

	/** AIの知覚刺激の発信を担当するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|AI")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	/** 守りに関するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Defense")
	TObjectPtr<UPSDefenseComponent> DefenseComponent;

	/** 攻撃に関するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Attack")
	TObjectPtr<UPSAttackComponent> AttackComponent;

protected:

	/** アビリティを管理するコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSAbilitySystemComponent> AbilitySystemComponent;

	/** 体力の属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSHealthAttributeSet> HealthAttributeSet;

	/** スタミナの属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSStaminaAttributeSet> StaminaAttributeSet;

	/** ブレイクの属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSBreakAttributeSet> BreakAttributeSet;

	/** 戦闘に関する属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSCombatAttributeSet> CombatAttributeSet;

	/** ステータス属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSStatusAttributeSet> StatusAttributeSet;

	/** 動きに関する属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSMovementAttributeSet> MovementAttributeSet;

	/** 重さに関する属性値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TObjectPtr<UPSWeightAttributeSet> WeightAttributeSet;

protected:

	/** キャラクターを構成するデータアセット */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Data")
	TObjectPtr<class UPSCharacterData> CharacterData;

public:

	/** GASコンポーネント取得 */
	virtual UPSAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	/** 体力の属性値取得 */
	virtual UPSHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; }
	/** スタミナの属性値を取得 */
	virtual UPSStaminaAttributeSet* GetStaminaAttributeSet() const { return StaminaAttributeSet; }
	/** ブレイクの属性値を取得 */
	virtual UPSBreakAttributeSet* GetBreakAttributeSet() const { return BreakAttributeSet; }
	/** 戦闘に関する属性値を取得 */
	virtual UPSCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet; }
	/** ステータス属性値を取得 */
	virtual UPSStatusAttributeSet* GetStatusAttributeSet() const { return StatusAttributeSet; }
	/** 動きに関する属性値を取得 */
	virtual UPSMovementAttributeSet* GetMovementAttributeSet() const { return MovementAttributeSet; }
	/** 重さに関する属性値を取得 */
	virtual UPSWeightAttributeSet* GetWeightAttributeSet() const { return WeightAttributeSet; }
	/** 装備コンポーネント取得 */
	virtual UPSEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	/** 守りコンポーネント取得 */
	virtual UPSDefenseComponent* GetDefenseComponent() const { return DefenseComponent; }
	/** 攻撃コンポーネント取得 */
	virtual UPSAttackComponent* GetAttackComponent() const { return AttackComponent; }
	/** データアセット取得 */
	virtual UPSCharacterData* GetCharacterData() const { return CharacterData; }

public:

	/** 着地したときのイベント取得 */
	TSharedRef<IPSObservable<FHitResult>> GetOnLandedEvent() const { return OnLandedEvent; }

	/** 空中にいるときのイベント取得 */
	TSharedRef<IPSObservable<Empty>> GetOnAirEvent() const { return OnAirEvent; }

protected:

	/** 着地したときのイベント */
	TSharedRef<TPSSubject<FHitResult>> OnLandedEvent = TPSSubject<FHitResult>::Create();

	/** 空中にいるときのイベント */
	TSharedRef<TPSSubject<Empty>> OnAirEvent = TPSSubject<Empty>::Create();

protected:

	/** コントローラがキャラクターを所持した瞬間に呼ばれる。初期化用 */
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	/** 着地したときに呼ばれる。イベント発行用 */
	virtual void Landed(const FHitResult& Hit) override;

	/** 移動モードが変化したときに呼ばれる。イベント発行用 */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:

	/** 基本的なアビリティを付与 */
	void GiveDefaultAbilities();

	/** 属性値を初期化 */
	void InitializeAttributes();
};
