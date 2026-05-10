// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGA_WeaponAttack.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/PSCharacterBase.h"
#include "Equipment/PSEquipmentComponent.h"
#include "Equipment/Weapons/PSWeaponBase.h"

//void UPSGA_WeaponAttack::ActivateAbility(
//	const FGameplayAbilitySpecHandle Handle,
//	const FGameplayAbilityActorInfo* ActorInfo,
//	const FGameplayAbilityActivationInfo ActivationInfo,
//	const FGameplayEventData* TriggerEventData 
//)
//{
//	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//
//
//	APSCharacterBase* Character = Cast<APSCharacterBase>(ActorInfo->AvatarActor.Get());
//	if (not Character)
//	{
//		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//		DEBUG_MESSAGE_WARNING("UPSGA_WeaponAttack::ActivateAbility - Character is null");
//		UE_LOG(LogTemp, Warning, TEXT("UPSGA_WeaponAttack::ActivateAbility - Character is null")); SET_LOG_PATH(Warning);
//		return;
//	}
//	APSWeaponBase* EquippedWeapon = Cast<APSWeaponBase>(Character->GetEquipmentComponent()->GetEquipmentActor(WeaponSlot));
//	if (not EquippedWeapon)
//	{
//		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//		DEBUG_MESSAGE_WARNING("UPSGA_WeaponAttack::ActivateAbility - EquippedWeapon is null");
//		UE_LOG(LogTemp, Warning, TEXT("UPSGA_WeaponAttack::ActivateAbility - EquippedWeapon is null")); SET_LOG_PATH(Warning);
//		return;
//	}
//
//	// 武器の当たり判定にイベントをバインド
//	EquippedWeapon->GetOnHitEvent()->Subscribe(this,
//		[this](AActor* OtherActor)
//		{
//			// 武器が当たったときの処理
//			OnWeaponHit(OtherActor);
//		}, nullptr,
//		[this]()
//		{
//			// 攻撃が終了したらアビリティを終了
//			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
//		});
//
//}
//
//void UPSGA_WeaponAttack::EndAbility(
//	const FGameplayAbilitySpecHandle Handle,
//	const FGameplayAbilityActorInfo* ActorInfo,
//	const FGameplayAbilityActivationInfo ActivationInfo,
//	bool bReplicateEndAbility, bool bWasCancelled)
//{
//	// アビリティ終了時のクリーンアップ処理があればここに追加
//
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}
//
//void UPSGA_WeaponAttack::OnWeaponHit(const FHitResult& HitResult)
//{
//	Super::OnWeaponHit(HitResult);
//}