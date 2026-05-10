// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "PSAnimNotifyState_SendEvent.generated.h"

/**
 * GASのGameplayEventを送るためのAnimNotifyState
 */
UCLASS()
class PROJECTSHIELD_API UPSAnimNotifyState_SendEvent : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	/** イベント開始時に送るタグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	FGameplayTag StartEventTag;

	/** イベント終了時に送るタグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	FGameplayTag EndEventTag;

	/** イベントのフレームごとに送るタグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PS|GAS")
	FGameplayTag TickEventTag;

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

};
