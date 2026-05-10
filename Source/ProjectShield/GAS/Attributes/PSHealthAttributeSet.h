// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Utility/Observer/PSSubject.h"
#include "PSHealthAttributeSet.generated.h"


/**
 * 体力の属性値を管理する
 */
UCLASS()
class PROJECTSHIELD_API UPSHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

    UPSHealthAttributeSet();

    /** 現在の体力 GameplayEffectによって直接値が更新されることはない */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "PS|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS_BASIC(UPSHealthAttributeSet, Health);

    /** 体力の最大値 */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "PS|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS_BASIC(UPSHealthAttributeSet, MaxHealth);

    /** 体力を回復する +Health */
    UPROPERTY(BlueprintReadOnly, Category = "PS|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Healing;
    ATTRIBUTE_ACCESSORS_BASIC(UPSHealthAttributeSet, Healing);

    /** ダメージ、体力を減らす -Health */
    UPROPERTY(BlueprintReadOnly, Category = "PS|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS_BASIC(UPSHealthAttributeSet, Damage);

public:

    /** 体力が更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnHealthUpdatedEvent() const { return OnHealthUpdatedEvent; }
    /** 体力の最大値が更新されたときのイベント取得 */
	TSharedRef<IPSObservable<float>> GetOnMaxHealthUpdatedEvent() const { return OnMaxHealthUpdatedEvent; }

protected:

    /** 体力が更新されたときのイベント */
    TSharedRef<TPSSubject<float>> OnHealthUpdatedEvent = TPSSubject<float>::Create();
    /** 体力の最大値が更新されたときのイベント */
	TSharedRef<TPSSubject<float>> OnMaxHealthUpdatedEvent = TPSSubject<float>::Create();

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

    /** サーバー側で Health が変更されたときに呼ばれる */
    UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldValue);

    /** サーバー側で MaxHealth が変更されたときに呼ばれる */
    UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

protected:

    /** 属性値の値を範囲内に制限 */
    void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;


};
