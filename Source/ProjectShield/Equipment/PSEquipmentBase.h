// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSEquipmentBase.generated.h"

UCLASS(Abstract)
class PROJECTSHIELD_API APSEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSEquipmentBase();

protected:

	/** 装備品のメッシュ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PS|Components")
	TObjectPtr<UStaticMeshComponent> EquipmentMesh;
};
