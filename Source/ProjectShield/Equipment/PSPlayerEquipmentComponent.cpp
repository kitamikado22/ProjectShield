// Fill out your copyright notice in the Description page of Project Settings.


#include "PSPlayerEquipmentComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Data/Item/PSEquipmentOnHandsData.h"
#include "Input/PSInputConfig.h"
#include "Equipment/PSEquipmentType.h"
#include "Singleton/Master/PSItemMasterManager.h"
#include "Utility/PSDebugMessageMacross.h"

UPSPlayerEquipmentComponent::UPSPlayerEquipmentComponent()
{
	// ハンドルの初期化
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseRightHand, FGameplayAbilitySpecHandle());
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseRightHand_Second, FGameplayAbilitySpecHandle());
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseLeftHand, FGameplayAbilitySpecHandle());
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseLeftHand_Second, FGameplayAbilitySpecHandle());
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseBothHands, FGameplayAbilitySpecHandle());
	EquipmentAbilityHandles.Add(EPSAbilityInputID::UseBothHands_Second, FGameplayAbilitySpecHandle());
}

void UPSPlayerEquipmentComponent::Equip(EEquipmentSlot Slot, FName ItemID)
{
	Super::Equip(Slot, ItemID);

	UPSItemMasterManager* ItemMaster = UPSGameInstanceSubsystem::Get<UPSItemMasterManager>(this);
	ItemMaster->GetOrLoadEquipmentData(ItemID, [ItemID, Slot, this](UPSEquipmentData* LoadedData)
		{
			// 指定スロットに装備できるか調べる
			if (CanEquipInSlot(Slot, LoadedData) == false)
			{
				return;
			}

			// 手に装備するタイプの装備品データか調べる
			UPSEquipmentOnHandsData* EquipmentOnHandsData = Cast<UPSEquipmentOnHandsData>(LoadedData);
			if (not EquipmentOnHandsData)
			{
				DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("The equipment data is not of type UPSEquipmentOnHandsData. Item Id is %s."), *LoadedData->ItemId.ToString()));
				UE_LOG(LogTemp, Warning, TEXT("The equipment data is not of type UPSEquipmentOnHandsData. Item Id is %s."), *LoadedData->ItemId.ToString()); SET_LOG_PATH(Warning);
				return;
			}

			if (Slot == EEquipmentSlot::RightHand)
			{
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseRightHand);
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseRightHand_Second);
			}
			else if (Slot == EEquipmentSlot::LeftHand)
			{
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseLeftHand);
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseLeftHand_Second);
			}

			// 両手に同じ武器を装備している場合
			if (Slot == EEquipmentSlot::RightHand && ItemID == GetEquipmentID(EEquipmentSlot::LeftHand)
				|| Slot == EEquipmentSlot::LeftHand && ItemID == GetEquipmentID(EEquipmentSlot::RightHand))
			{
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseBothHands);
				GiveEquipmentAbility(EquipmentOnHandsData, EPSAbilityInputID::UseBothHands_Second);
			}
		});
}

void UPSPlayerEquipmentComponent::RemoveEquipment(EEquipmentSlot Slot)
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (not ASI)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to get AbilitySystemInterface from owner."));
		UE_LOG(LogTemp, Error, TEXT("Failed to get AbilitySystemInterface from owner.")); SET_LOG_PATH(Error);
		return;
	}
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (not ASC)
	{
		DEBUG_MESSAGE_ERROR(TEXT("Failed to get AbilitySystemComponent from AbilitySystemInterface."));
		UE_LOG(LogTemp, Error, TEXT("Failed to get AbilitySystemComponent from AbilitySystemInterface.")); SET_LOG_PATH(Error);
		return;
	}

	if (Slot == EEquipmentSlot::RightHand)
	{
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseRightHand]);
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseRightHand_Second]);
	}
	else if (Slot == EEquipmentSlot::LeftHand)
	{
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseLeftHand]);
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseLeftHand_Second]);
	}

	if (GetEquipmentID(EEquipmentSlot::RightHand) == GetEquipmentID(EEquipmentSlot::LeftHand))
	{
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseBothHands]);
		ASC->ClearAbility(EquipmentAbilityHandles[EPSAbilityInputID::UseBothHands_Second]);
	}

	Super::RemoveEquipment(Slot);
}

void UPSPlayerEquipmentComponent::GiveEquipmentAbility(UPSEquipmentOnHandsData* EquipmentData, EPSAbilityInputID AbilityInputID)
{
	if (not EquipmentData->AbilitySoftClassMap.Contains(AbilityInputID))
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("There is no ability class for the specified input ID in the equipment data. Input ID is %d."), (int32)AbilityInputID));
		UE_LOG(LogTemp, Warning, TEXT("There is no ability class for the specified input ID in the equipment data. Input ID is %d."), (int32)AbilityInputID); SET_LOG_PATH(Warning);
		return;
	}

	if (not EquipmentData->AbilitySoftClassMap[AbilityInputID].ToSoftObjectPath().IsValid())
	{
		DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("The ability class for the specified input ID is not valid in the equipment data. Input ID is %d."), (int32)AbilityInputID));
		UE_LOG(LogTemp, Warning, TEXT("The ability class for the specified input ID is not valid in the equipment data. Input ID is %d."), (int32)AbilityInputID); SET_LOG_PATH(Warning);
		return;
	}

	UPSItemMasterManager* ItemMaster = UPSGameInstanceSubsystem::Get<UPSItemMasterManager>(this);
	ItemMaster->LoadClass(EquipmentData->AbilitySoftClassMap[AbilityInputID], [this, AbilityInputID](UClass* LoadedClass)
		{
			if (not LoadedClass)
			{
				DEBUG_MESSAGE_ERROR(TEXT("Failed to load right hand ability class. Class is %s."));
				UE_LOG(LogTemp, Error, TEXT("Failed to load right hand ability class.")); SET_LOG_PATH(Error);
				return;
			}
			if (LoadedClass->IsChildOf(UGameplayAbility::StaticClass()) == false)
			{
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Loaded class is not a subclass of UGameplayAbility. Class is %s."), *LoadedClass->GetName()));
				UE_LOG(LogTemp, Error, TEXT("Loaded class is not a subclass of UGameplayAbility. Class is %s."), *LoadedClass->GetName()); SET_LOG_PATH(Error);
				return;
			}

			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
			if (not ASI)
			{
				DEBUG_MESSAGE_ERROR(TEXT("Failed to get AbilitySystemInterface from owner."));
				UE_LOG(LogTemp, Error, TEXT("Failed to get AbilitySystemInterface from owner.")); SET_LOG_PATH(Error);
				return;
			}

			UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (not ASC)
			{
				DEBUG_MESSAGE_ERROR(TEXT("Failed to get AbilitySystemComponent from AbilitySystemInterface."));
				UE_LOG(LogTemp, Error, TEXT("Failed to get AbilitySystemComponent from AbilitySystemInterface.")); SET_LOG_PATH(Error);
				return;
			}

			EquipmentAbilityHandles[AbilityInputID] = ASC->GiveAbility(FGameplayAbilitySpec(LoadedClass, 1, static_cast<int32>(AbilityInputID)));

		});
}