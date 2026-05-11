// Fill out your copyright notice in the Description page of Project Settings.


#include "PSLockOnComponent.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PSCharacterBase.h"
#include "Enemy/PSEnemyCharacterBase.h"
#include "Utility/PSDebugMessageMacross.h"

UPSLockOnComponent::UPSLockOnComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPSLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (not CurrentTarget)
    {
        return;
    }


    APSCharacterBase* OwnerCharacter = Cast<APSCharacterBase>(GetOwner());
    AController* Controller = OwnerCharacter->GetController();

    FVector DirectionToTarget = CurrentTarget->GetActorLocation() - OwnerCharacter->GetActorLocation();
    FRotator RotationToTarget = DirectionToTarget.Rotation();
    FRotator CurrentRotation = Controller->GetControlRotation();

    RotationToTarget.Pitch = CurrentRotation.Pitch;
    RotationToTarget.Roll = CurrentRotation.Roll;

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, RotationToTarget, DeltaTime, 10.f);

    Controller->SetControlRotation(NewRotation);
}

void UPSLockOnComponent::LockOn(AActor* NewTarget)
{
    CurrentTarget = NewTarget;
}

void UPSLockOnComponent::UnLock()
{
    CurrentTarget = nullptr;
}

void UPSLockOnComponent::ToggleLockOn()
{
    // ターゲットをロックオンしていれば、ロックオンを無効化
    if (CurrentTarget)
    {
        UnLock();
    }
    // ターゲットがいなければ、ロックオンを有効化
    else
    {
        AActor* Target = FindTarget();
        if (Target)
        {
            LockOn(Target);
        }
    }

    // イベント配信
    OnLockOnEvent->OnNext(CurrentTarget);
}

bool UPSLockOnComponent::IsLockedOn() const
{
    return CurrentTarget ? true : false;
}

AActor* UPSLockOnComponent::FindTarget()
{
    APSCharacterBase* OwnerCharacter = Cast<APSCharacterBase>(GetOwner());
    if (not OwnerCharacter)
    {
        DEBUG_MESSAGE_ERROR(TEXT("Owner is not type of APSCharacterBase"));
        UE_LOG(LogTemp, Log, TEXT("Owner is not type of APSCharacterBase"));
        return nullptr;
    }

    FVector Start = OwnerCharacter->GetActorLocation();
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(OwnerCharacter);
    TArray<AActor*> OutActors;

    // 物理的にプレイヤーに近いアクタを取得
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        Start,
        FindDistance,
        ObjectTypes,
        nullptr,
        IgnoreActors,
        OutActors);

    // 近くにアクタがいなければ終了
    if (OutActors.IsEmpty())
    {
        return nullptr;
    }

    APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
    
    int32 SizeX;
    int32 SizeY;
    PC->GetViewportSize(SizeX, SizeY);
    FVector2D ViewportCenter = FVector2D(SizeX * 0.5f, SizeY * 0.5f);

    // 画面中央からの距離、物理的な距離などからスコアを出して、候補を絞る
    // スコアは小さいほどターゲットに選択される
    float BestScore = FLT_MAX;

    AActor* BestTarget = nullptr;

    for (AActor* Target : OutActors)
    {
        // 敵キャラクター型の子クラスではない場合、敵ではないので終了
        if (not Target->IsA<APSEnemyCharacterBase>())
        {
            continue;
        }

        // 3D空間から2D画面の座標に変換
        FVector2D ScreenPos;
        bool bProjected = UGameplayStatics::ProjectWorldToScreen(PC, Target->GetActorLocation(), ScreenPos);
        if (not bProjected)
        {
            continue;
        }

        // 画面中央との距離
        float ScreenDistance = FVector2D::Distance(ScreenPos, ViewportCenter);
        // 物理距離
        float WorldDistance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());

        // スコア（重みを付けて画面の中央に近いほうが少し有利）
        float Score = ScreenDistance * 0.7f + WorldDistance * 0.3f;

        // ベストターゲット更新
        if (Score < BestScore)
        {
            BestScore = Score;
            BestTarget = Target;
        }
    }

    DEBUG_MESSAGE(FString::Printf(TEXT("Lock on!  Target Actor is %s"), *BestTarget->GetName()));
    UE_LOG(LogTemp, Log, TEXT("Lock on!  Target Actor is %s"), *BestTarget->GetName());

    if (not BestTarget)
    {
        DEBUG_MESSAGE_WARNING(TEXT("There were no enemies to target"));
        UE_LOG(LogTemp, Log, TEXT("There were no enemies to target"));
    }

    return BestTarget;
}
