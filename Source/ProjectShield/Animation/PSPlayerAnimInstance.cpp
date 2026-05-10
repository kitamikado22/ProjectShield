// Fill out your copyright notice in the Description page of Project Settings.


#include "PSPlayerAnimInstance.h"
#include "Character/PSCharacterBase.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/PSGameplayTags.h"

void UPSPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPSPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (not IsValid(Character))
	{
		return;
	}
	
	// ダッシュしているかを更新
	bIsSprinting = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(PSGameplayTags::State_Sprint)
		&& Character->GetVelocity().Size() > 600.f;

	// 入力によって動かそうとしているかを更新
	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	if (not CharacterMovement)
	{
		return;
	}
	bIsInputMoving = CharacterMovement->GetCurrentAcceleration().SizeSquared() > 0.f;

	// ダッシュジャンプをしているかを更新
	if (not bIsUpdateSprintJumping)
	{
		// ダッシュしている状態で空中にいる場合
		if (bIsSprinting && bIsInAir)
		{
			bIsSprintJumping = true;
		}
		// ダッシュしていない状態で空中にいる場合
		else if (not bIsSprinting && bIsInAir)
		{
			bIsSprintJumping = false;
		}
		bIsUpdateSprintJumping = true;
	}
	// 地面についたら更新フラグをリセット
	else if (not bIsInAir)
	{
		bIsUpdateSprintJumping = false;
	}
	
	// ガードしているかを更新
	bIsGuarding = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(PSGameplayTags::State_Guard);
	// 受け流しをしているかを更新
	bIsDeflecting = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(PSGameplayTags::State_Deflect);
	// 両手を使っているかを更新
	bIsUsingBothHanded = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(PSGameplayTags::State_UseBothHands);
}