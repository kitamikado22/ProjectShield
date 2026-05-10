// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_Sprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "Character/PSCharacterBase.h"
#include "Player/PSPlayerCharacter.h"
#include "Utility/PSDebugMessageMacross.h"
#include "GAS/PSGameplayTags.h"


UPSGA_Sprint::UPSGA_Sprint()
{
    StaminaCostAmount = 0.f;

	FGameplayTagContainer AssetTags = GetAssetTags();
    AssetTags.AddTag(PSGameplayTags::Ability_Sprint);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(PSGameplayTags::State_Sprint);
}

bool UPSGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (not Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // プレイヤーはスティックの入力が閾値以上でないとダッシュできない
    if (APSPlayerCharacter* Player = Cast<APSPlayerCharacter>(GetAvatarActorFromActorInfo()))
    {
        if (Player->GetMoveInput().Size() < SprintStickThreshold)
        {
            return false;
        }
    }
    
    return true;
}

void UPSGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


    APSCharacterBase* Character = Cast<APSCharacterBase>(GetAvatarActorFromActorInfo());
    if (not Character)
    {
        DEBUG_MESSAGE_ERROR(TEXT("AvatarActor is not of type APSCharacterBase"));
        UE_LOG(LogTemp, Error, TEXT("AvatarActor is not of type APSCharacterBase")); SET_LOG_PATH(Error);
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
	}

    if (not StaminaDrainEffect)
    {
        DEBUG_MESSAGE_ERROR(TEXT("StaminaDrainEffect is not set"));
        UE_LOG(LogTemp, Error, TEXT("StaminaDrainEffect is not set")); SET_LOG_PATH(Error);
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 移動速度を上げる（CharacterMovementへアクセス）
	// TODO: 後にGameplayEffectで移動速度を上げる方法も検討
    Character->GetCharacterMovement()->MaxWalkSpeed = 900.f; // ダッシュ速度

    // スタミナ消費GEを適用
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(StaminaDrainEffect, GetAbilityLevel());
    if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
    {
        DrainHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

	// スタミナの変化イベントを購読
    StaminaUpdateDisposable = Character->GetStaminaAttributeSet()->GetOnStaminaUpdatedEvent()->Subscribe(this, [this](float NewValue)
        {
            if (NewValue <= 0.f)
            {
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
        });

    // 空中にいるときのイベント購読
    AirDisposable = Character->GetOnAirEvent()->Subscribe(this, [this](const Empty& Empty)
        {
            // スプリントジャンプアビリティを発動させて終了
			GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(SprintJumpAbilityClass);

            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		});

    // NOTE: アビリティはプレイヤーでなくても動くことを考慮するためEndAbilityは行わない
    APSPlayerCharacter* Player = Cast<APSPlayerCharacter>(Character);
    if (not Player)
    {
        DEBUG_MESSAGE_ERROR(TEXT("OwnerCharacter is not of type APSPlayerCharacter"));
        UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is not of type APSPlayerCharacter")); SET_LOG_PATH(Error);
		return;
    }

	// スティック入力イベント購読
    InputMoveDisposable = Player->GetOnMoveEvent()->Subscribe(this, [this](const FVector2D& MoveInput)
        {
            if (MoveInput.Size() < SprintStickThreshold)
            {

                const float StopThreshold = 0.4f;

                // プレイヤーがスティックを急に止めた場合
                if (MoveInput.Size() < StopThreshold)
                {
                    // ストップする（ストップモーションを再生）
                    ToStop();
                }

                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
            }
		});
}

void UPSGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 移動速度を元に戻す
    if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = 600.f; // 通常速度
    }

    // スタミナ消費GEを解除
    if (DrainHandle.IsValid())
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(DrainHandle);
    }

	// スタミナ変化イベントの購読を解除
    if (StaminaUpdateDisposable.IsValid())
    {
        StaminaUpdateDisposable->Dispose();
    }
	// スティック入力イベントの購読を解除
    if (InputMoveDisposable.IsValid())
    {
		InputMoveDisposable->Dispose();
	}
	// 空中にいるイベントの購読を解除
    if (AirDisposable.IsValid())
    {
        AirDisposable->Dispose();
	}

    // スタミナ自然回復遅延GameEffectを適用
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        if (StaminaRegenDelayEffectClass)
        {
            ASC->ApplyGameplayEffectToSelf(
                StaminaRegenDelayEffectClass.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());
        }
    }


    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPSGA_Sprint::ToStop()
{
    if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
    {
        // ストップモーションを再生
        Character->PlayAnimMontage(ToStopMontage);
    }
}