// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Blackbordで使用するキーの文字列をまとめ
 */
namespace PSBBKeys
{
	const FName TargetActor = TEXT("TargetActor");	// ターゲットのアクター
	const FName DistanceToTarget = TEXT("DistanceToTarget");	// ターゲットとの距離
	const FName IsInCombat = TEXT("IsInCombat");	// 戦闘中か
	const FName IsStaggered = TEXT("IsStaggered");	// 怯み状態か
	const FName Phase = TEXT("Phase");	// フェーズ
	
}