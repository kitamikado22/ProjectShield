// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Utility/Observer/PSSubject.h"
#include "PSBreakAttributeSet.generated.h"

/**
 * ブレイクゲージの属性セット
 */
UCLASS()
class PROJECTSHIELD_API UPSBreakAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPSBreakAttributeSet();
	
	/** ブレイク値 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Break", ReplicatedUsing = OnRep_Break, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Break;
	ATTRIBUTE_ACCESSORS_BASIC(UPSBreakAttributeSet, Break);

	/** ピークブレイク値 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Break", ReplicatedUsing = OnRep_MaxBreak, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxBreak;
	ATTRIBUTE_ACCESSORS_BASIC(UPSBreakAttributeSet, MaxBreak);

	/** ブレイクゲージを回復 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Break", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Recover;
	ATTRIBUTE_ACCESSORS_BASIC(UPSBreakAttributeSet, Recover);

	/** ブレイクゲージを悪化 */
	UPROPERTY(BlueprintReadOnly, Category = "PS|Break", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Worsen;
	ATTRIBUTE_ACCESSORS_BASIC(UPSBreakAttributeSet, Worsen);

public:

	/** ブレイク値が更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnBreakUpdatedEvent() const { return OnBreakUpdatedEvent; }
	/** ブレイク値の最大値が更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnMaxBreakUpdatedEvent() const { return OnMaxBreakUpdatedEvent; }
	/** ブレイク値が悪化したときのイベント取得 */
	TSharedRef<IPSObservable<FGameplayEffectModCallbackData>> GetOnWorsonUpdateEvent() const { return OnWorsonUpdateEvent; }

protected:

	/** ブレイク値が更新されたときのイベント */
	TSharedRef<TPSSubject<float>> OnBreakUpdatedEvent = TPSSubject<float>::Create();
	/** ブレイク値の最大値が更新されたときのイベント */
	TSharedRef<TPSSubject<float>> OnMaxBreakUpdatedEvent = TPSSubject<float>::Create();
	/** ブレイク値が悪化したときのイベント */
	TSharedRef<TPSSubject<FGameplayEffectModCallbackData>> OnWorsonUpdateEvent = TPSSubject<FGameplayEffectModCallbackData>::Create();

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

	/** サーバー側で Posture が変更されたときに呼ばれる */
	UFUNCTION() void OnRep_Break(const FGameplayAttributeData& OldValue);

	/** サーバー側で MaxPosture が変更されたときに呼ばれる */
	UFUNCTION() void OnRep_MaxBreak(const FGameplayAttributeData& OldValue);

protected:

	/** 属性値の値を範囲内に制限 */
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

};
