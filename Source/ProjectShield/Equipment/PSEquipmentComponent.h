// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipment/PSEquipmentType.h"
#include "Data/Item/PSEquipmentData.h"
#include "PSEquipmentComponent.generated.h"


/** 
 * キャラクターの装備に関するコンポーネント
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSHIELD_API UPSEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPSEquipmentComponent();

protected:

	/** スケルタルメッシュのソケットデータ */
	TObjectPtr<class UPSSocketData> SocketData;

	/** 装備スロットとアタッチするソケット名のペアデータ */
	TMap<EEquipmentSlot, FName> EquipmentSocketMap;

protected:

	/** 現在の装備品のデータ */
	TMap<EEquipmentSlot, TObjectPtr<UPSEquipmentData>> EquipmentMap;

	/** 現在の装備品のアクタ */
	TMap<EEquipmentSlot, TObjectPtr<APSEquipmentBase>> EquipmentActors;

public:

	/** 配列データから複数のアイテムを一度に装備 */
	virtual void EquipByMap(const TArray<FEquipmentIDAndSlot>& Equipments);

	/** 指定スロットに指定のアイテムを装備する */
	virtual void Equip(EEquipmentSlot Slot, FName ItemID);

	/** 装備解除 */
	virtual void RemoveEquipment(EEquipmentSlot Slot);

	/** 指定スロットに装備しているアイテム情報を取得 */
	UPSEquipmentData* GetEquipmentData(EEquipmentSlot Slot) const;

	/** 現在使っている装備のアイテム情報を取得 */
	UPSEquipmentData* GetUsedEquipmentData() const;

	/** 指定スロットに装備している装備品アクタを取得 */
	APSEquipmentBase* GetEquipmentActor(EEquipmentSlot Slot) const;

	/** 指定スロットに装備している装備品のIDを取得 */
	FName GetEquipmentID(EEquipmentSlot Slot) const;

protected:

	/** 指定スロットに装備品を装備できるか調べる */
	bool CanEquipInSlot(EEquipmentSlot Slot, const UPSEquipmentData* EquipmentData) const;

protected:

	virtual void BeginPlay() override;

};
