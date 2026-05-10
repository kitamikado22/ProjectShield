// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHIELD_API UPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UPSAbilitySystemComponent();

protected:

	/** タグ付与用のGE */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> DynamicTagEffectClass;

	/** 現在このアビリティで付与されているタグとハンドルのマップデータ */
	TMap<FGameplayTag, FActiveGameplayEffectHandle> ActiveDynamicTagHandles;

public:

	/** 持ち主に動的にタグを付与させる */
	FActiveGameplayEffectHandle AddDynamicTagToOwner(FGameplayTag Tag);

	/** 指定したタグを削除 */
	void RemoveDynamicTagFromOwner(FGameplayTag Tag);
};
