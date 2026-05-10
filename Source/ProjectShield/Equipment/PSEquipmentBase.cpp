// Fill out your copyright notice in the Description page of Project Settings.


#include "PSEquipmentBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APSEquipmentBase::APSEquipmentBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 装備品のメッシュコンポーネントを作成
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	RootComponent = EquipmentMesh;

	// 当たり判定はクエリのみ（物理衝突はなし）
	EquipmentMesh->SetCollisionProfileName(TEXT("EquipmentPreset"));
	EquipmentMesh->SetSimulatePhysics(false);
}

