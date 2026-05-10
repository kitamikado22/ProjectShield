// Fill out your copyright notice in the Description page of Project Settings.

#include "PSEquipmentComponent.h"
#include "GameFramework/Character.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Utility/PSMultiLoadManager.h"
#include "Singleton/Master/PSItemMasterManager.h"
#include "Singleton/Master/PSSocketMasterManager.h"
#include "Equipment/PSEquipmentBase.h"
#include "Data/PSSocketData.h"
#include "Character/PSCharacterBase.h"
#include "GAS/PSAbilitySystemComponent.h"
#include "GAS/PSGameplayTags.h"

UPSEquipmentComponent::UPSEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPSEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ソケット名のマップデータを取得
	if (USkeletalMesh* SkeltalMesh = CastChecked<ACharacter>(GetOwner())->GetMesh()->GetSkeletalMeshAsset())
	{
		FName SKMName(SkeltalMesh->GetName());	// スケルタルメッシュの名前を取得

		UPSSocketMasterManager* SocketMaster = UPSGameInstanceSubsystem::Get<UPSSocketMasterManager>(this);
		auto Handle = SocketMaster->GetOrLoadEquipmentSocketMap(SKMName, [this](UPSSocketData* LoadedData)
		{
			SocketData = LoadedData;
			EquipmentSocketMap = LoadedData->EquipmentSocketMap;
		});
		

		UPSMultiLoadManager::GetInstance(this)->AddToLoadBlock("CharacterStartup", SKMName, Handle);
	}
}

void UPSEquipmentComponent::EquipByMap(const TArray<FEquipmentIDAndSlot>& Equipments)
{
	// 渡された装備スロットとアイテムIDのセット情報から、全て装備
	for (const FEquipmentIDAndSlot& Equipment : Equipments)
	{
		Equip(Equipment.Slot, Equipment.ItemID);
	}
}

void UPSEquipmentComponent::Equip(EEquipmentSlot Slot, FName ItemId)
{
	if (ItemId.IsNone())
	{
		RemoveEquipment(Slot);
		return;
	}

	// キーが存在するか調べる
	if (EquipmentMap.Contains(Slot))
	{
		// 何か装備している場合
		if (EquipmentMap.FindRef(Slot))
		{
			// すでに同じ装備であるなら
			if (ItemId == EquipmentMap[Slot]->ItemId)
			{
				DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("This weapon is already equipped. Item Id is %s."), *ItemId.ToString()));
				UE_LOG(LogTemp, Warning, TEXT("This weapon is already equipped. Item Id is %s."), *ItemId.ToString()); SET_LOG_PATH(Warning);
				return;
			}
			// すでに別の武器を装備しているなら
			else
			{
				RemoveEquipment(Slot);
			}
		}
		
	}

	// 指定の装備品データを取得・ロード
	UPSItemMasterManager* ItemMaster = UPSGameInstanceSubsystem::Get<UPSItemMasterManager>(this);
	ItemMaster->GetOrLoadEquipmentData(ItemId, [Slot, this](UPSEquipmentData* LoadedData)
	{
		// 指定スロットに装備できるか調べる
		if (CanEquipInSlot(Slot, LoadedData) == false)
		{
			return;
		}

		EquipmentMap.Emplace(Slot, LoadedData);

		// 装備品アクタのクラス型をロードして取得
		UPSItemMasterManager* ItemMaster = UPSGameInstanceSubsystem::Get<UPSItemMasterManager>(this);
		ItemMaster->LoadClass(LoadedData->EquipmentClass, [Slot, this](UClass* LoadedEquipmentClass)
		{
			// 装備データのロードエラーなどで失敗している場合
			if (not EquipmentMap.Contains(Slot))
			{
				DEBUG_MESSAGE_ERROR(TEXT("There is no equipment data in the specified slot."));
				UE_LOG(LogTemp, Error, TEXT("There is no equipment data in the specified slot.")); SET_LOG_PATH(Error);
				return;
			}

			AActor* Owner = GetOwner();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Owner;
			SpawnParams.Instigator = Owner->GetInstigator();

			// 装備品アクタを実体化スポーン
			APSEquipmentBase* SpawnedEquipment = GetWorld()->SpawnActor<APSEquipmentBase>(LoadedEquipmentClass, Owner->GetActorLocation(), Owner->GetActorRotation(), SpawnParams);

			if (SpawnedEquipment == nullptr)
			{
				
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Equipment actor spawn failed. Equipment is %s."), *EquipmentMap[Slot]->ItemId.ToString()));
				UE_LOG(LogTemp, Error, TEXT("Equipment actor spawn failed. Equipment is %s."), *EquipmentMap[Slot]->ItemId.ToString()); SET_LOG_PATH(Error);
				return;
			}

			// スケルタルメッシュコンポーネントを取得
			USkeletalMeshComponent* CharacterMesh = CastChecked<ACharacter>(Owner)->GetMesh();
			check(CharacterMesh);

			if (not EquipmentSocketMap.Contains(Slot))
			{
				DEBUG_MESSAGE_ERROR(TEXT("The specified slot does not have a socket name."));
				UE_LOG(LogTemp, Error, TEXT("The specified slot does not have a socket name.")); SET_LOG_PATH(Error);
				return;
			}

			if (not CharacterMesh->DoesSocketExist(EquipmentSocketMap[Slot]))
			{
				DEBUG_MESSAGE_ERROR(FString::Printf(TEXT("Failed to attach to socket. Socket name is %s."), *EquipmentSocketMap[Slot].ToString()));
				UE_LOG(LogTemp, Error, TEXT("Failed to attach to socket. Socket name is %s."), *EquipmentSocketMap[Slot].ToString()); SET_LOG_PATH(Error);
				return;
			}

			// 装備品をキャラクターのソケットにアタッチする
			SpawnedEquipment->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipmentSocketMap[Slot]);

			// オフセット値を反映
			if (EquipmentMap[Slot]->EquipOffsets.Contains(Slot))
			{
				SpawnedEquipment->SetActorRelativeTransform(EquipmentMap[Slot]->EquipOffsets[Slot]);
			}

			EquipmentActors.Emplace(Slot, SpawnedEquipment);
			DEBUG_MESSAGE(FString::Printf(TEXT("The equipment was successful. Equipment is %s."), *EquipmentMap[Slot]->ItemId.ToString()));
			UE_LOG(LogTemp, Log, TEXT("The equipment was successful. Equipment is %s."), *EquipmentMap[Slot]->ItemId.ToString());
		});
	});
}

bool UPSEquipmentComponent::CanEquipInSlot(EEquipmentSlot Slot, const UPSEquipmentData* EquipmentData) const
{
	for (const EEquipmentSlot& EquippableSlot : EquipmentData->EquippableSlots)
	{
		if (Slot == EquippableSlot)
		{
			// 装備できると判断
			return true;
		}
	}

	// 指定スロットには装備できない
	DEBUG_MESSAGE_WARNING(FString::Printf(TEXT("Could not equip in specified slot. Equipment is %s."), *EquipmentData->ItemId.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("Could not equip in specified slot. Equipment is %s."), *EquipmentData->ItemId.ToString()); SET_LOG_PATH(Warning);
	return false;
}

void UPSEquipmentComponent::RemoveEquipment(EEquipmentSlot Slot)
{
	// 装備品アクタを破棄
	if (EquipmentActors.Contains(Slot))
	{
		if (EquipmentActors.FindRef(Slot))
		{
			EquipmentActors[Slot]->Destroy();
			EquipmentActors.Emplace(Slot, nullptr);
		}
	}

	// 装備品情報を破棄
	if (EquipmentMap.Contains(Slot))
	{
		if (EquipmentMap.FindRef(Slot))
		{
			EquipmentMap.Emplace(Slot, nullptr);
		}
	}
}

UPSEquipmentData* UPSEquipmentComponent::GetEquipmentData(EEquipmentSlot Slot) const
{
	if (EquipmentMap.Contains(Slot))
	{
		if (UPSEquipmentData* EquipmentData = EquipmentMap[Slot])
		{
			return EquipmentData;
		}
	}

	DEBUG_MESSAGE_WARNING(TEXT("There was no equipment data in the specified slot."));
	UE_LOG(LogTemp, Warning, TEXT("There was no equipment data in the specified slot.")); SET_LOG_PATH(Warning);
	return nullptr;
}

UPSEquipmentData* UPSEquipmentComponent::GetUsedEquipmentData() const
{
	APSCharacterBase* OwnerCharacter = CastChecked<APSCharacterBase>(GetOwner());
	if (not OwnerCharacter)
	{
		DEBUG_MESSAGE_ERROR(TEXT("The owner of this component is not a PSCharacterBase."));
		UE_LOG(LogTemp, Error, TEXT("The owner of this component is not a PSCharacterBase.")); SET_LOG_PATH(Error);
		return nullptr;
	}
	
	UPSAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	
	// 右手を使っている場合
	if (ASC->HasMatchingGameplayTag(PSGameplayTags::State_UseRightHand))
	{
		return GetEquipmentData(EEquipmentSlot::RightHand);
	}
	// 左手を使っている場合
	else if (ASC->HasMatchingGameplayTag(PSGameplayTags::State_UseLeftHand))
	{
		return GetEquipmentData(EEquipmentSlot::LeftHand);
	}
	// 両手を使っている場合
	else if (ASC->HasMatchingGameplayTag(PSGameplayTags::State_UseBothHands))
	{
		return GetEquipmentData(EEquipmentSlot::RightHand);
	}
	// 何も使っていない場合
	else
	{
		return nullptr;
	}
}

APSEquipmentBase* UPSEquipmentComponent::GetEquipmentActor(EEquipmentSlot Slot) const
{
	if (EquipmentActors.Contains(Slot))
	{
		if (APSEquipmentBase* EquipmentActor = EquipmentActors[Slot])
		{
			return EquipmentActor;
		}
	}
	
	DEBUG_MESSAGE_WARNING(TEXT("There was no equipment actor in the specified slot."));
	UE_LOG(LogTemp, Warning, TEXT("There was no equipment actor in the specified slot.")); SET_LOG_PATH(Warning);
	return nullptr;
}

FName UPSEquipmentComponent::GetEquipmentID(EEquipmentSlot Slot) const
{
	if (EquipmentMap.Contains(Slot))
	{
		if (UPSEquipmentData* EquipmentData = EquipmentMap[Slot])
		{
			return EquipmentData->ItemId;
		}
	}

	DEBUG_MESSAGE_WARNING(TEXT("There was no equipment data in the specified slot."));
	UE_LOG(LogTemp, Warning, TEXT("There was no equipment data in the specified slot.")); SET_LOG_PATH(Warning);
	return NAME_None;
}