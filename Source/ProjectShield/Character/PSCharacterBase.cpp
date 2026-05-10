// Fill out your copyright notice in the Description page of Project Settings.


#include "PSCharacterBase.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "GAS/Attributes/PSHealthAttributeSet.h"
#include "GAS/Attributes/PSStaminaAttributeSet.h"
#include "GAS/Attributes/PSBreakAttributeSet.h"
#include "GAS/Attributes/PSCombatAttributeSet.h"
#include "GAS/Attributes/PSStatusAttributeSet.h"
#include "GAS/Attributes/PSMovementAttributeSet.h"
#include "GAS/Attributes/PSWeightAttributeSet.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/Character/PSCharacterData.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Utility/PSMultiLoadManager.h"
#include "Components/PSDefenseComponent.h"
#include "Components/PSAttackComponent.h"


APSCharacterBase::APSCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// 装備品コンポーネントを作成
	EquipmentComponent = ObjectInitializer.CreateDefaultSubobject<UPSEquipmentComponent>(this, TEXT("EquipmentComponent"));

	// アビリティを制御するコンポーネントを作成
	AbilitySystemComponent = CreateDefaultSubobject<UPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// 守りに関するコンポーネントを作成
	DefenseComponent = CreateDefaultSubobject<UPSDefenseComponent>(TEXT("DefenseComponent"));
	// 攻撃に関するコンポーネントを作成
	AttackComponent = CreateDefaultSubobject<UPSAttackComponent>(TEXT("AttackComponent"));

	// 属性値を作成
	HealthAttributeSet = CreateDefaultSubobject<UPSHealthAttributeSet>(TEXT("HealthAttributeSet"));
	StaminaAttributeSet = CreateDefaultSubobject<UPSStaminaAttributeSet>(TEXT("StaminaAttributeSet"));
	BreakAttributeSet = CreateDefaultSubobject<UPSBreakAttributeSet>(TEXT("BreakAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UPSCombatAttributeSet>(TEXT("CombatAttributeSet"));
	StatusAttributeSet = CreateDefaultSubobject<UPSStatusAttributeSet>(TEXT("StatusAttributeSet"));
	MovementAttributeSet = CreateDefaultSubobject<UPSMovementAttributeSet>(TEXT("MovementAttributeSet"));
	WeightAttributeSet = CreateDefaultSubobject<UPSWeightAttributeSet>(TEXT("WeightAttributeSet"));

	// AIの知覚刺激の発信を担当するコンポーネントを作成
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->bAutoRegister = true;

}

void APSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// ソケットのマップデータがロードされるのを待ってから装備の初期化を行う
	if (CharacterData)
	{
		FName SocketLoader = FName(GetMesh()->GetSkeletalMeshAsset()->GetName());
		UPSMultiLoadManager::GetInstance(this)->SubscribeToLoadBlock(this, "CharacterStartup", { SocketLoader }, [this]()
			{
				// 装備の初期化
				EquipmentComponent->EquipByMap(CharacterData->DefaultEquipments);
			});
	}


	
}

void APSCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 基本的なアビリティを付与
		GiveDefaultAbilities();

		// 属性値の初期化
		InitializeAttributes();
	}
}

void APSCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	// 着地イベントを配信
	OnLandedEvent->OnNext(Hit);
}

void APSCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	// 空中にいるときイベントを配信
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		OnAirEvent->OnNext(Empty::None);
	}
}

void APSCharacterBase::GiveDefaultAbilities()
{
	if (not AbilitySystemComponent || not CharacterData)
	{
		DEBUG_MESSAGE_ERROR(TEXT("AbilitySystemComponent or CharacterData is null. Cannot initialize attributes."));
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent or CharacterData is null. Cannot initialize attributes.")); SET_LOG_PATH(Error);
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterData->PassiveAbilityClasses)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
			AbilitySystemComponent->GiveAbilityAndActivateOnce(Spec);
		}
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterData->DefaultAbilitys)
	{
		if (AbilityClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this)
			);
		}
	}
}

void APSCharacterBase::InitializeAttributes()
{
	if (not AbilitySystemComponent || not CharacterData)
	{
		DEBUG_MESSAGE_ERROR(TEXT("AbilitySystemComponent or CharacterData is null. Cannot initialize attributes."));
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent or CharacterData is null. Cannot initialize attributes.")); SET_LOG_PATH(Error);
		return;
	}

	// サーバー側でのみ初期化を行う
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (const FAttributeDefaultValue& Pair : CharacterData->DefaultAttributes)
	{
		if (Pair.Attribute.IsValid())
		{
			AbilitySystemComponent->SetNumericAttributeBase(Pair.Attribute, Pair.Value);
		}
		else
		{
			DEBUG_MESSAGE_ERROR(TEXT("Invalid attribute in CharacterData. Cannot initialize attributes."));
			UE_LOG(LogTemp, Error, TEXT("Invalid attribute in CharacterData. Cannot initialize attributes.")); SET_LOG_PATH(Error);
		}
	}
}