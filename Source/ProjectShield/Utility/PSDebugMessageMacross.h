// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

/** 画面出力デバッグメッセージ */
#define DEBUG_MESSAGE(msg) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, msg)
#define DEBUG_MESSAGE_COLOR(msg, color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.0f, color, msg)
#define DEBUG_MESSAGE_ERROR(msg) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, msg)
#define DEBUG_MESSAGE_WARNING(msg) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Yellow, msg)
#define DEBUG_TEST_MESSAGE(msg) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Magenta, msg)

/** ログ出力場所のパスを残す */
#define SET_LOG_PATH(verb) UE_LOG(LogTemp, verb, TEXT("%s(%d)"), *FString(__FILE__), __LINE__);

