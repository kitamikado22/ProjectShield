// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PSCharacterBase.h"
#include "Utility/Observer/PSSubject.h"
#include "Data/Character/PSEnemyData.h"
#include "PSEnemyCharacterBase.generated.h"

/**
 * 敵キャラクターの基底クラス
 * プレイヤーには共通しないが、敵キャラクターに必要な機能をここに実装する
 */
UCLASS(Abstract)
class PROJECTSHIELD_API APSEnemyCharacterBase : public APSCharacterBase
{
	GENERATED_BODY()

public:

	APSEnemyCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:

	/** ターゲットプレイヤー */
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTargetPlayer;

public:

	/** プレイヤーを見つける */
	void EngagePlayer(AActor* TargetPlayer);

	/** プレイヤーを見失う */
	void DisengagePlayer();

public:

	/** 敵のデータを取得 */
	virtual UPSEnemyData* GetCharacterData() const override { return Cast<UPSEnemyData>(CharacterData); }

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** コントローラがキャラクターを所持した瞬間に呼ばれる。初期化用 */
	virtual void PossessedBy(AController* NewController) override;

};
