// Fill out your copyright notice in the Description page of Project Settings.

#include "PSPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameplayEffectExtension.h"
#include "Components/PSLockOnComponent.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "GAS/Attributes/PSHealthAttributeSet.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "GAS/Attributes/PSBreakAttributeSet.h"
#include "GAS/Abilities/PSGA_Sprint.h"
#include "GAS/Abilities/PSGA_Jump.h"
#include "GAS/Abilities/PSGA_Roll.h"
#include "GAS/Abilities/PSGA_LockOn.h"
#include "GAS/Abilities/PSGA_SprintJump.h"
#include "GAS/Abilities/PSGA_StaminaRegen.h"
#include "GAS/Abilities/PSGA_HitReaction.h"
#include "GAS/PSGameplayEffectContext.h"
#include "GAS/PSAbilitySystemGlobals.h"
#include "GAS/PSGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "Equipment/PSPlayerEquipmentComponent.h"
#include "Singleton/Master/PSItemMasterManager.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Utility/PSMultiLoadManager.h"
#include "PSPlayerController.h"
#include "UI/PSMainHUDWidget.h"
#include "Data/Character/PSPlayerData.h"


APSPlayerCharacter::APSPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPSPlayerEquipmentComponent>(TEXT("EquipmentComponent")))
{
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーを示すタグを追加
	Tags.Add(FName("Player"));

	// コントローラが回転したときキャラクターは回転しない
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	// キャラクターの移動方向に自動的に回転

	// カメラを制御するバネの作成と設定
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bDoCollisionTest = true;
	SpringArmComp->ProbeChannel = ECC_Camera;

	// カメラの作成と設定
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 100.f;

	// ロックオンコンポーネントを作成
	LockOnComponent = CreateDefaultSubobject<UPSLockOnComponent>(TEXT("LockOnComponent"));

}

void APSPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D CurrentMoveInput = Value.Get<FVector2D>();
	MoveInput = CurrentMoveInput;

	if (GetController())
	{
		// コントローラの回転からヨー成分のみを取り出して水平方向の前方・右方向ベクトルを算出する
		const FRotator ControlRotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, CurrentMoveInput.Y);
		AddMovementInput(RightDirection, CurrentMoveInput.X);

		OnMoveEvent->OnNext(CurrentMoveInput);
	}
}

void APSPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void APSPlayerCharacter::OnPressedUseRightHand()
{
	AbilityInputPressed(EPSAbilityInputID::UseRightHand);
}
void APSPlayerCharacter::OnReleasedUseRightHand()
{
	AbiilityInputReleased(EPSAbilityInputID::UseRightHand);
}

void APSPlayerCharacter::OnPressedUseRightHand_Second()
{
	AbilityInputPressed(EPSAbilityInputID::UseRightHand_Second);
}
void APSPlayerCharacter::OnReleasedUseRightHand_Second()
{
	AbiilityInputReleased(EPSAbilityInputID::UseRightHand_Second);
}

void APSPlayerCharacter::OnPressedUseLeftHand()
{
	AbilityInputPressed(EPSAbilityInputID::UseLeftHand);
}
void APSPlayerCharacter::OnReleasedUseLeftHand()
{
	AbiilityInputReleased(EPSAbilityInputID::UseLeftHand);
}

void APSPlayerCharacter::OnPressedUseLeftHand_Second()
{
	AbilityInputPressed(EPSAbilityInputID::UseLeftHand_Second);
}
void APSPlayerCharacter::OnReleasedUseLeftHand_Second()
{
	AbiilityInputReleased(EPSAbilityInputID::UseLeftHand_Second);
}

void APSPlayerCharacter::OnPressedUseBothHands()
{
	AbilityInputPressed(EPSAbilityInputID::UseBothHands);
}
void APSPlayerCharacter::OnReleasedUseBothHands()
{
	AbiilityInputReleased(EPSAbilityInputID::UseBothHands);
}

void APSPlayerCharacter::OnPressedUseBothHands_Second()
{
	AbilityInputPressed(EPSAbilityInputID::UseBothHands_Second);
}
void APSPlayerCharacter::OnReleasedUseBothHands_Second()
{
	AbilityInputPressed(EPSAbilityInputID::UseBothHands_Second);
}

void APSPlayerCharacter::OnPressedJump()
{
	AbilityInputPressed(EPSAbilityInputID::Jump);
}

void APSPlayerCharacter::OnPressedSprint()
{
	AbilityInputPressed(EPSAbilityInputID::Sprint);
}

void APSPlayerCharacter::OnPressedRoll()
{
	AbilityInputPressed(EPSAbilityInputID::Roll);
}

void APSPlayerCharacter::OnPressedLockOn()
{
	AbilityInputPressed(EPSAbilityInputID::LockOn);
}

void APSPlayerCharacter::AbilityInputPressed(EPSAbilityInputID InputID)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));
}

void APSPlayerCharacter::AbiilityInputReleased(EPSAbilityInputID InputID)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(InputID));
}

void APSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 値などの初期化
	SpringArmComp->TargetArmLength = GetCharacterData()->TargetArmLength;
	SpringArmComp->SocketOffset = GetCharacterData()->NormalCameraOffset;

	// 体力・スタミナ・体幹が変更したときのイベントをUI側に通知
	GetHealthAttributeSet()->GetOnHealthUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIの体力バーを更新
				Controller->GetMainHUDWidget()->GetHealthBar()->UpdateValue(NewValue);
			}
		});
	GetHealthAttributeSet()->GetOnMaxHealthUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIの体力バーの最大値を更新
				Controller->GetMainHUDWidget()->GetHealthBar()->UpdateMaxValue(NewValue);
			}
		});
	GetStaminaAttributeSet()->GetOnStaminaUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIのスタミナバーを更新
				Controller->GetMainHUDWidget()->GetStaminaBar()->UpdateValue(NewValue);
			}
		});
	GetStaminaAttributeSet()->GetOnMaxStaminaUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIのスタミナバーの最大値を更新
				Controller->GetMainHUDWidget()->GetStaminaBar()->UpdateMaxValue(NewValue);
			}
		});
	GetBreakAttributeSet()->GetOnBreakUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIのブレイクゲージを更新
				Controller->GetMainHUDWidget()->GetBreakBar()->UpdateValue(NewValue);
			}
		});
	GetBreakAttributeSet()->GetOnMaxBreakUpdatedEvent()->Subscribe(this, [this](float NewValue)
		{
			if (APSPlayerController* Controller = Cast<APSPlayerController>(GetController()))
			{
				// UIのブレイクゲージの最大値を更新
				Controller->GetMainHUDWidget()->GetBreakBar()->UpdateMaxValue(NewValue);
			}
		});

	// UIに初期値を設定
	if (APSPlayerController* PC = Cast<APSPlayerController>(GetController()))
	{
		PC->GetMainHUDWidget()->GetHealthBar()->UpdateValue(GetHealthAttributeSet()->GetHealth());
		PC->GetMainHUDWidget()->GetHealthBar()->UpdateMaxValue(GetHealthAttributeSet()->GetMaxHealth());
		PC->GetMainHUDWidget()->GetStaminaBar()->UpdateValue(GetStaminaAttributeSet()->GetStamina());
		PC->GetMainHUDWidget()->GetStaminaBar()->UpdateMaxValue(GetStaminaAttributeSet()->GetMaxStamina());
		PC->GetMainHUDWidget()->GetBreakBar()->UpdateValue(GetBreakAttributeSet()->GetBreak());
		PC->GetMainHUDWidget()->GetBreakBar()->UpdateMaxValue(GetBreakAttributeSet()->GetMaxBreak());
	}

	// ブレイク値が悪化したときヒットリアクションアビリティを発動
	GetBreakAttributeSet()->GetOnWorsonUpdateEvent()->Subscribe(this, [this](const FGameplayEffectModCallbackData& Data)
		{
			OnWorsonUpdate(Data);
		});

	//// ロックオンが有効・無効化したときのイベントを購読
	//LockOnComponent->GetOnLockOnEvent()->Subscribe(this, [this](const TWeakObjectPtr<AActor>& InTargetActor)
	//	{
	//		OnLockOnUpdate(InTargetActor.Get());
	//	});
}

void APSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// カメラやロックオン時のマーカーなどを設定
	UpdateCamera(DeltaTime);
}

void APSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 現在所持しているアビリティを付与
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// ダッシュアビリティ
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			GetCharacterData()->SprintAbilityClass, 1, static_cast<int32>(EPSAbilityInputID::Sprint)));
		// ジャンプアビリティ
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			GetCharacterData()->JumpAbilityClass, 1, static_cast<int32>(EPSAbilityInputID::Jump)));
		// ローリングアビリティ
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			GetCharacterData()->RollAbilityClass, 1, static_cast<int32>(EPSAbilityInputID::Roll)));
		// ロックオンアビリティ
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			GetCharacterData()->LockOnAbilityClass, 1, static_cast<int32>(EPSAbilityInputID::LockOn)));

		// 入力無効タグが更新されたときのイベント購読
		AbilitySystemComponent->RegisterGameplayTagEvent(
			PSGameplayTags::State_DisableInput,
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &APSPlayerCharacter::OnDisableInputTagChanged);
	}
}

void APSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APSPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APSPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APSPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedJump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedSprint);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedRoll);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedLockOn);
		EnhancedInputComponent->BindAction(UseRightHandAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseRightHand);
		EnhancedInputComponent->BindAction(UseRightHandAction, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseRightHand);
		EnhancedInputComponent->BindAction(UseRightHandAction_Second, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseRightHand_Second);
		EnhancedInputComponent->BindAction(UseRightHandAction_Second, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseRightHand_Second);
		EnhancedInputComponent->BindAction(UseLeftHandAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseLeftHand);
		EnhancedInputComponent->BindAction(UseLeftHandAction, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseLeftHand);
		EnhancedInputComponent->BindAction(UseLeftHandAction_Second, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseLeftHand_Second);
		EnhancedInputComponent->BindAction(UseLeftHandAction_Second, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseLeftHand_Second);
		EnhancedInputComponent->BindAction(UseBothHandsAction, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseBothHands);
		EnhancedInputComponent->BindAction(UseBothHandsAction, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseBothHands);
		EnhancedInputComponent->BindAction(UseBothHandsAction_Second, ETriggerEvent::Started, this, &APSPlayerCharacter::OnPressedUseBothHands_Second);
		EnhancedInputComponent->BindAction(UseBothHandsAction_Second, ETriggerEvent::Completed, this, &APSPlayerCharacter::OnReleasedUseBothHands_Second);
	}
}

void APSPlayerCharacter::OnWorsonUpdate(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == UPSBreakAttributeSet::GetWorsenAttribute())
	{
		FGameplayEffectContext* Context = Data.EffectSpec.GetContext().Get();
		if (not Context)
		{
			DEBUG_MESSAGE_ERROR(TEXT("Context is null"));
			UE_LOG(LogTemp, Error, TEXT("Context is null")); SET_LOG_PATH(Error);
			return;
		}

		FHitResult* HitResult = Context->GetHitResult();
		if (not HitResult)
		{
			DEBUG_MESSAGE_ERROR(TEXT("HitResult is null"));
			UE_LOG(LogTemp, Error, TEXT("HitResult is null")); SET_LOG_PATH(Error);
			return;
		}

		// 吹っ飛ばす力の大きさ
		float ImpactPower = Data.EvaluatedData.Magnitude;

		// 攻撃がヒットしたことを衝撃力とともにイベント配信
		FGameplayEventData Payload;
		Payload.Instigator = Context->GetInstigator();
		Payload.Target = this;
		Payload.EventTag = PSGameplayTags::Event_Attack_Hit;
		Payload.EventMagnitude = ImpactPower;

		// ヒットデータをイベントに渡す
		FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit();
		HitData->HitResult = *HitResult;
		Payload.TargetData.Add(HitData);
		// コンテキストデータをイベントに渡す
		Payload.ContextHandle = Data.EffectSpec.GetContext();

		GetAbilitySystemComponent()->HandleGameplayEvent(PSGameplayTags::Event_Attack_Hit, &Payload);
	}
}

void APSPlayerCharacter::OnDisableInputTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	APSPlayerController* PC = Cast<APSPlayerController>(GetController());
	if (not PC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to cast GetController to PlayerController"));
		UE_LOG(LogTemp, Error, TEXT("Failed to cast GetController to PlayerController")); SET_LOG_PATH(Error);
		return;
	}

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (not LocalPlayer)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Loacal Player is null"));
		UE_LOG(LogTemp, Error, TEXT("Loacal Player is null")); SET_LOG_PATH(Error);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (not Subsystem)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Enhanced Input Local Player Subsystem is null"));
		UE_LOG(LogTemp, Error, TEXT("Enhanced Input Local Player Subsystem is null")); SET_LOG_PATH(Error);
		return;
	}

	// 入力無効タグが追加された場合
	if (NewCount > 0)
	{
		Subsystem->RemoveMappingContext(PC->GetIMC_Movement());	// 移動に関する入力を排除
		Subsystem->RemoveMappingContext(PC->GetIMC_Combat());	// 戦闘に関する入力を排除
	}
	else
	{
		Subsystem->AddMappingContext(PC->GetIMC_Movement(), 10);	// 移動に関する入力を再適用
		Subsystem->AddMappingContext(PC->GetIMC_Combat(), 20);	// 戦闘に関する入力を再適用
	}
}

void APSPlayerCharacter::UpdateCamera(float DeltaTime)
{
	APSPlayerController* PC = Cast<APSPlayerController>(GetController());
	if (not PC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to cast GetController to PlayerController"));
		UE_LOG(LogTemp, Error, TEXT("Failed to cast GetController to PlayerController")); SET_LOG_PATH(Error);
		return;
	}

	// 目標のオフセット
	FVector DesiredOffset;

	// ロックオン状態
	if (LockOnComponent->IsLockedOn())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		DesiredOffset = GetCharacterData()->LockOnCameraOffset;

	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		DesiredOffset = GetCharacterData()->NormalCameraOffset;

	}

	// カメラを滑らかにオフセット変更
	SpringArmComp->SocketOffset = FMath::VInterpTo(SpringArmComp->SocketOffset, DesiredOffset, DeltaTime, 10.f);

	// HUDでロックオンしている敵をマークする
	PC->GetMainHUDWidget()->MarkLockOnPointerToTarget(LockOnComponent->GetCurrentTarget());
}