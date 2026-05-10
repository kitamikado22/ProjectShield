// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/PSEnemyCharacterBase.h"
#include "PSOrcCharacter.generated.h"

/**
 * オーク。暴信の追放者
 */
UCLASS()
class PROJECTSHIELD_API APSOrcCharacter : public APSEnemyCharacterBase
{
	GENERATED_BODY()
	
public:

	APSOrcCharacter(const FObjectInitializer& ObjectInitializer);
};
