// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/PSEquipmentBase.h"
#include "GameFramework/Actor.h"
#include "Utility/Observer/PSSubject.h"
#include "PSWeaponBase.generated.h"

/** 
* 武器の基底クラス
*/
UCLASS(Abstract)
class PROJECTSHIELD_API APSWeaponBase : public APSEquipmentBase
{
	GENERATED_BODY()
	
public:	

	APSWeaponBase();

private:

	/** 攻撃がヒットしたときのイベント */
	TSharedRef<TPSSubject<AActor*>> OnHitEventSubject = TPSSubject<AActor*>::Create();

public:

	/** 攻撃がヒットしたときのイベント取得 */
	TSharedPtr<IPSObservable<AActor*>> GetOnHitEvent() const { return OnHitEventSubject->AsObservable(); }

protected:

	/** 武器の刃などの太さ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Weapon")
	float CapsuleRadius = 5.0f;

	/** 武器の根元のソケット名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Weapon")
	FName RootSocket = TEXT("Root");

	/** 武器の先端のソケット名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Weapon")
	FName TipSocket = TEXT("Tip");

private:

	/** 武器の刃などの当たり判定の形状 */
	FCollisionShape AttackCollisionShape;

private:

	bool bIsAttacking = false;

	/** 一度の攻撃において、多段ヒット防止 */
	TSet<TWeakObjectPtr<AActor>> HitActors;

	/** 前のフレームでの、武器の座標 */
	FVector PreviousRootPosition;
	FVector PreviousTipPosition;

public:

	/** 攻撃を開始 当たり判定のトレースを開始 */
	UFUNCTION()
	void StartAttack();
	
	/** 攻撃を終了 当たり判定のトレースを終了 */
	UFUNCTION()
	void EndAttack();

private:

	/** フレーム間での武器の軌道をトレースして当たり判定をする */
	void ExecuteAttackTrace();

public:

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

};
