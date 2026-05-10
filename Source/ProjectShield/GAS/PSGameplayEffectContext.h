// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "PSGameplayEffectContext.generated.h"

/** 
* GameplayEffectと通信するためのコンテキスト構造体
* 主に攻撃処理などの通信役として使用する
*/
USTRUCT(BlueprintType)
struct FPSGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FGameplayTag, float> GenericDataMap;


	/** スクリプト構造体を返す */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FPSGameplayEffectContext::StaticStruct();
	}

	/** コピー処理 */
	virtual FPSGameplayEffectContext* Duplicate() const override
	{
		FPSGameplayEffectContext* NewContext = new FPSGameplayEffectContext(*this);
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** ネットワーク同期用 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override
	{
		return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
	}
};

template<>
struct TStructOpsTypeTraits<FPSGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPSGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};