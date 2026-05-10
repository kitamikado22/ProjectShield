// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PSItemData.generated.h"

/**
 * このゲームで存在できるアイテムのデータ
 */
UCLASS()
class PROJECTSHIELD_API UPSItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/** アイテムの識別子 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Item")
	FName ItemId;

	/** 表示されるアイテムの名前 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Item")
	FText DisplayName;

	/** アイテムの説明 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Item")
	FString Description;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", ItemId);
	}
};
