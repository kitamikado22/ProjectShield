// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PSExecutionCalculation.generated.h"

/**
 * GameplayEffectの実行計算を行う基底クラス
 * 主にヘルパー関数などを提供
 */
UCLASS(Abstract)
class PROJECTSHIELD_API UPSExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
};

/** 変数名を指定して属性値をキャプチャ宣言できる */
#define DEFINE_ATTRIBUTE_CAPTUREDEF_NAMED(StructType, AttributeName, CaptureSource, bSnapshot, PropertyName) \
{ \
    PropertyName##Property = FindFieldChecked<FProperty>( \
        StructType::StaticClass(), \
        GET_MEMBER_NAME_CHECKED(StructType, AttributeName)); \
    \
    PropertyName##Def = FGameplayEffectAttributeCaptureDefinition( \
        PropertyName##Property, \
        EGameplayEffectAttributeCaptureSource::CaptureSource, \
        bSnapshot); \
}