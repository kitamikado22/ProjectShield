// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PSGameplayAbility.generated.h"

/**
 * プロジェクトで扱うGameplayAbilityの基底クラス
 */
UCLASS()
class PROJECTSHIELD_API UPSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPSGameplayAbility();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** タグ付与用のGE */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	TSubclassOf<UGameplayEffect> DynamicTagEffectClass;

	/** 現在このアビリティで付与されているタグとハンドルのマップデータ */
	TMap<FGameplayTag, FActiveGameplayEffectHandle> ActiveDynamicTagHandles;

protected:

	/** 持ち主に動的にタグを付与させる */
	FActiveGameplayEffectHandle AddDynamicTagToOwner(FGameplayTag Tag);

	/** 指定したタグを削除 */
	void RemoveDynamicTagFromOwner(FGameplayTag Tag);
};
