// Fill out your copyright notice in the Description page of Project Settings.

#include "PSAttackComponent.h"
#include "Utility/PSDebugMessageMacross.h"
#include "DrawDebugHelpers.h" // デバッグ描画用

UPSAttackComponent::UPSAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

int32 UPSAttackComponent::StartAttackTrace(UMeshComponent* MeshComponent, FName StartSocket, FName EndSocket, float Radius, TFunction<void(const FHitResult&)>OnHit)
{
    if (not MeshComponent)
    {
        DEBUG_MESSAGE_ERROR(TEXT("StartAttackTrace: MeshComponent is null."));
        UE_LOG(LogTemp, Error, TEXT("StartAttackTrace: MeshComponent is null.")); SET_LOG_PATH(Error);
        return -1;
    }

    // ソケットが存在するか確認
	if (not MeshComponent->DoesSocketExist(StartSocket) || not MeshComponent->DoesSocketExist(EndSocket))
    {
		DEBUG_MESSAGE_WARNING(TEXT("StartAttackTrace: Invalid socket names provided."));
		UE_LOG(LogTemp, Warning, TEXT("StartAttackTrace: Invalid socket names provided.")); SET_LOG_PATH(Warning);
        return -1;
	}

	FPSAttackTraceRequest NewRequest;
	NewRequest.ID = NextRequestID++;
	NewRequest.MeshComponent = MeshComponent;
	NewRequest.StartSocket = StartSocket;
	NewRequest.EndSocket = EndSocket;
	NewRequest.Radius = Radius;
	NewRequest.bIsFirstFrame = true;
	NewRequest.OnHit = OnHit;

	ActiveRequests.Add(NewRequest);
	return NewRequest.ID;
}

void UPSAttackComponent::EndAttackTrace(int32 RequestID)
{
	int8 RemovedCount = ActiveRequests.RemoveAll([RequestID](const FPSAttackTraceRequest& Request)
		{
			return Request.ID == RequestID;
		});

	if (RemovedCount == 0)
    {
        DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("EndAttackTrace: No active request found with ID %d."), RequestID));
        UE_LOG(LogTemp, Warning, TEXT("EndAttackTrace: No active request found with ID %d."), RequestID); SET_LOG_PATH(Warning);
	}
}

void UPSAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ActiveRequests.Num() == 0)
    {
        return;
    }

    for (FPSAttackTraceRequest& Request : ActiveRequests)
    {
		USceneComponent* Mesh = Request.MeshComponent;
        if (not Mesh)
        {
            DEBUG_MESSAGE_ERROR(TEXT("TickComponent: MeshComponent is null in active request."));
            UE_LOG(LogTemp, Error, TEXT("TickComponent: MeshComponent is null in active request.")); SET_LOG_PATH(Error);
            continue;
		}

        FVector CurrentStart = Mesh->GetSocketLocation(Request.StartSocket);
        FVector CurrentEnd = Mesh->GetSocketLocation(Request.EndSocket);

        // 最初のフレームは前フレーム位置がないため保存してスキップ
        if (Request.bIsFirstFrame)
        {
            Request.LastStartPos = CurrentStart;
            Request.LastEndPos = CurrentEnd;
            Request.bIsFirstFrame = false;
            continue;
        }

        TArray<FHitResult> OutHits;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetOwner());
        for (auto& WeakActor : Request.HitActors) 
        {
            if (WeakActor.IsValid()) Params.AddIgnoredActor(WeakActor.Get());
        }

		// カプセルの中心位置の計算
        FVector CurrentCenter = (CurrentEnd + CurrentStart) * 0.5f;
        FVector LastCenter = (Request.LastEndPos + Request.LastStartPos) * 0.5f;

        // カプセルの向きの計算
		FQuat Rotation = FRotationMatrix::MakeFromZ(CurrentEnd - CurrentStart).ToQuat();

		// カプセルの形状を作成
        float HalfHeight = FVector::Dist(CurrentEnd, CurrentStart) * 0.5f;
		FCollisionShape Shape = FCollisionShape::MakeCapsule(Request.Radius, HalfHeight);

		// スイープ実行
        bool bHit = GetWorld()->SweepMultiByChannel(
            OutHits,
            LastCenter,
            CurrentCenter,
            Rotation,
            ECC_GameTraceChannel1,  // 攻撃用のトレースチャンネル
            Shape,
            Params
        );

        if (bHit)
        {
            for (const FHitResult& Hit : OutHits)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor && not Request.HitActors.Contains(HitActor))
                {
                    Request.HitActors.Add(HitActor);
                    
                    if (Request.OnHit.IsSet())
                    {
                        Request.OnHit(Hit);
                    }
                }
            }
        }

#if WITH_EDITOR
        // デバッグ描画
        FColor DebugColor = bHit ? FColor::Green : FColor::Red;
        DrawDebugCapsule(GetWorld(), CurrentCenter, HalfHeight, Request.Radius, Rotation, DebugColor, false, 1.0f);
#endif 

        Request.LastStartPos = CurrentStart;
        Request.LastEndPos = CurrentEnd;
    }
}