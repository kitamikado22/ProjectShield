// Fill out your copyright notice in the Description page of Project Settings.


#include "PSDefaultAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Utility//PSDebugMessageMacross.h"
#include "Character/PSCharacterBase.h"

void UPSDefaultAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

    APawn* Pawn = TryGetPawnOwner();
    if (not Pawn) return;

    Character = Cast<APSCharacterBase>(Pawn);
    if (not Character)
    {
		DEBUG_MESSAGE_ERROR("Pawn is not a PSCharacter.");
        UE_LOG(LogTemp, Error, TEXT("Pawn is not a PSCharacter.")); SET_LOG_PATH(Error);
		return;
    }

}

void UPSDefaultAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (not IsValid(Character))
    {
		DEBUG_MESSAGE_ERROR("Character is not valid");
		UE_LOG(LogTemp, Error, TEXT("Character is not valid.")); SET_LOG_PATH(Error);
        return;
    }

    UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
    if (not IsValid(CharacterMovement))
    {
        DEBUG_MESSAGE_ERROR("CharacterMovementComponent is not valid");
        UE_LOG(LogTemp, Error, TEXT("CharacterMovementComponent is not valid.")); SET_LOG_PATH(Error);
        return;
	}

    // 速度を更新
    FVector Velocity = Character->GetVelocity();
    Speed = Velocity.Size();

    // 視線に対する向きを更新
    FRotator ActorRotation = Character->GetActorRotation();
    Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);

    // 動いているかを更新
	bIsMoving = Speed > 0.1f; // 速度が0.01以上なら動いているとみなす

    // 加速しているかを更新
	Acceleration = (Speed - PreviousSpeed) / DeltaSeconds;
    bIsAccelerating = Acceleration > 0.1f;

    // 空中にいるかを更新
	bIsInAir = CharacterMovement->IsFalling();

	// 落ちているかを更新
    bIsFalling = bIsInAir && Velocity.Z < -0.1f;

    // 1フレーム前の速度を保存
	PreviousSpeed = Speed;
    

	// UE_LOG(LogTemp, Log, TEXT("SpeedNormalized: %f, Direction: %f, ShouldMove: %s, By: %s"), SpeedNormalized, Direction, ShouldMove ? TEXT("true") : TEXT("false"), *Character->GetName()); SET_LOG_PATH(Log);
}