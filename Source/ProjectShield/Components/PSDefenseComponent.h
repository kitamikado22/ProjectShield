// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSDefenseComponent.generated.h"

/**
 * 守り行動の種類
 */
UENUM()
enum class EPSDefenseType : uint8
{
	None,		// なし
	Guard,		// 受け止める
	Deflect,	// 受け流す
	Parry,		// 受け返す
};

/** 
 * 守りなどに関するコンポーネント
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSHIELD_API UPSDefenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** 攻撃に対して守り行動を決める */
	EPSDefenseType DecideDefenseType(const FHitResult& HitResult);
};
