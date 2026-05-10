// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWeaponBase.h"
#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Kismet/KismetSystemLibrary.h" // KismetSystemLibrary を使用するためのインクルード


APSWeaponBase::APSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APSWeaponBase::BeginPlay()
{
    Super::BeginPlay();

	// 当たり判定の形状を設定
    FVector RootPos = EquipmentMesh->GetSocketLocation(RootSocket);
	FVector TipPos = EquipmentMesh->GetSocketLocation(TipSocket);
	AttackCollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, (TipPos - RootPos).Size() * 0.5f);
}

void APSWeaponBase::StartAttack()
{
	// 攻撃をしている最中なら何もしない
	if (bIsAttacking)
	{
		return;
	}

	// イベントの配信を初期化
	OnHitEventSubject->Reset();

	bIsAttacking = true;
	HitActors.Empty();

	// 初期位置を保存
	PreviousRootPosition = EquipmentMesh->GetSocketLocation(RootSocket);
	PreviousTipPosition = EquipmentMesh->GetSocketLocation(TipSocket);
}

void APSWeaponBase::EndAttack()
{
	bIsAttacking = false;
	HitActors.Empty();

	// 攻撃が終了したことを通知
	OnHitEventSubject->OnCompleted();

	// イベントの配信を初期化
	OnHitEventSubject->Reset();
}

void APSWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		ExecuteAttackTrace();
	}
}

void APSWeaponBase::ExecuteAttackTrace()
{
    TArray<FHitResult> HitResults;

	// 現在の位置を取得
	FVector CurrentTipPos = EquipmentMesh->GetSocketLocation(TipSocket);

    // スイープさせる形状を回転
    //FVector Delta = CurrentTipPos - PreviousTipPosition;

    // Kismet に渡す無視アクタ配列を作成
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(this);
    if (GetOwner())
    {
        IgnoreActors.Add(GetOwner());
    }

	// 武器用のトレースチャンネル
	ETraceTypeQuery WeaponTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	// CapsuleTraceMulti を使用して、前フレームから現在位置までのカプセル軌道を検出
	bool bHit = UKismetSystemLibrary::CapsuleTraceMulti(
		GetWorld(),
		PreviousTipPosition,
		CurrentTipPos,
		CapsuleRadius,
		AttackCollisionShape.GetCapsuleHalfHeight(),
		WeaponTraceType,
		false,	// bTraceComplex
		IgnoreActors,
		EDrawDebugTrace::ForDuration,	// デバッグ描画を有効
		HitResults,
		true,	// bIgnoreSelf
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f
	);

    if (bHit)
    {
		for (const auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && not HitActors.Contains(HitActor))
			{
				DEBUG_MESSAGE(FString::Printf(TEXT("Hit Actor: %s"), *HitActor->GetName()));
				UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());

				// 多段ヒット防止のために記録
				HitActors.Add(HitActor);

				// ヒットイベントを発行
				OnHitEventSubject->OnNext(HitActor);
			}
		}
    }

}

