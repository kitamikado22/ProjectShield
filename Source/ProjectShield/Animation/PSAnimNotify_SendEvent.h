// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "PSAnimNotify_SendEvent.generated.h"

/**
 * GASのGameplayEventを送るAnimNotify
 */
UCLASS()
class PROJECTSHIELD_API UPSAnimNotify_SendEvent : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	/** 送るタグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	FGameplayTag EventTag;

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
