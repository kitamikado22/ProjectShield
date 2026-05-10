// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Utility/Observer/PSSubject.h"
#include "PSStaminaAttributeSet.generated.h"

/**
 * スタミナの属性値を管理する
 */
UCLASS()
class PROJECTSHIELD_API UPSStaminaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSStaminaAttributeSet();

	/** スタミナ */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "PS|Stamina", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStaminaAttributeSet, Stamina);

	/** スタミナの最大値 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "PS|Stamina", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UPSStaminaAttributeSet, MaxStamina);

public:

	/** スタミナが更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnStaminaUpdatedEvent() const { return OnStaminaUpdatedEvent; }
	/** スタミナの最大値が更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnMaxStaminaUpdatedEvent() const { return OnMaxStaminaUpdatedEvent; }

protected:

	/** スタミナが更新されたときのイベント */
	TSharedRef<TPSSubject<float>> OnStaminaUpdatedEvent = TPSSubject<float>::Create();
	/** スタミナの最大値が更新されたときのイベント */
	TSharedRef<TPSSubject<float>> OnMaxStaminaUpdatedEvent = TPSSubject<float>::Create();

protected:

	/** GemeplayEffectの処理が完了する 前に 呼ばれる */
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;

	/** GameplayEffectの処理が完了した 後に 呼ばれる */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/** ローカルで属性値が変更される 前に 呼ばれる */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** ローカルで属性値が変更された 後に 呼ばれる */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/** サーバーと同期させたい属性値を登録 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** サーバー側で Stamina が変更されたときに呼ばれる */
	UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	/** サーバー側で MaxStamina が変更されたときに呼ばれる */
	UFUNCTION() void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

protected:

	/** 属性値の値を範囲内に制限 */
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

};
