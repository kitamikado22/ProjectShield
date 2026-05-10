// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSLockOnComponent.generated.h"

/** 
 * ロックオンのロジックを担当するコンポーネント 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSHIELD_API UPSLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

    /** ターゲットの候補にいれる物理的な距離 */
    float FindDistance = 1500.f;

public:

    /** ロックオンを有効化・無効化するトグル */
    void ToggleLockOn();

protected:

    /** 指定のターゲットをロックオン */
    void LockOn(AActor* NewTarget);

    /** 現在のターゲットをロックオン解除 */
    void UnLock();

    /** 現在の状況から最適なターゲットを一人決める */
    AActor* FindTarget();

protected:

    /** 現在のターゲット */
    TObjectPtr<AActor> CurrentTarget;

};
