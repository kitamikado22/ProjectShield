// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PSAttackComponent.generated.h"

/** 当たり判定処理のリクエストデータ */
USTRUCT(BlueprintType)
struct FPSAttackTraceRequest
{
	GENERATED_BODY()

	int32 ID;
	UMeshComponent* MeshComponent;
	FName StartSocket;
	FName EndSocket;
	float Radius;
	TArray<TWeakObjectPtr<AActor>> HitActors;

	FVector LastStartPos;
	FVector LastEndPos;

	bool bIsFirstFrame = true;

	TFunction<void(const FHitResult&)> OnHit;
};

/** 
 * 攻撃に関するコンポーネント
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSHIELD_API UPSAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPSAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 攻撃開始 IDを返し、アビリティ側はこのIDで終了命令を出す */
	int32 StartAttackTrace(UMeshComponent* MeshComponent, FName StartSocket, FName EndSocket, float Radius, TFunction<void(const FHitResult&)>OnHit);

	/** 攻撃終了 */
	void EndAttackTrace(int32 RequestID);

private:

	/** 現在有効になっているリクエスト */
	TArray<FPSAttackTraceRequest> ActiveRequests;

	int32 NextRequestID = 0;
};
