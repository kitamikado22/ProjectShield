// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Utility/PSDebugMessageMacross.h"
#include "Utility/PSAssetManager.h"

void UPSAudioManager::Initialize(FSubsystemCollectionBase& Collection)
{

    Super::Initialize(Collection);
    CurrentBGMComponent = nullptr;
    MemorizedFieldBGM = nullptr;

    // サウンドミックスの設定を全てリセット
    // UGameplayStatics::ClearSoundMixModifiers(GetWorld());

    if (MainSoundMix)
    {
        UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
    }
    else
    {
        DEBUG_MESSAGE_ERROR(TEXT("MainSoundMix is ​​not valid"));
        UE_LOG(LogTemp, Log, TEXT("MainSoundMix is ​​not valid"));
    }

    UpdateMasterVolume(0.1f);
    UpdateBGMVolume(0.1f);
    UpdateSFXVolume(0.9f);
}

void UPSAudioManager::Deinitialize()
{
    if (CurrentBGMComponent)
    {
        CurrentBGMComponent->Stop();
        CurrentBGMComponent = nullptr;
    }
    Super::Deinitialize();
}

void UPSAudioManager::PlayEnemyBGM(TSoftObjectPtr<USoundBase> InBGM, float FadeInTime)
{
    if (InBGM.IsNull())
    {
        DEBUG_MESSAGE_ERROR(TEXT("InBGM is null"));
        UE_LOG(LogTemp, Log, TEXT("InBGM is null")); SET_LOG_PATH(Error);
        return;
    }

    // 現在再生中のBGMがあればフェードアウトし、元の曲を記憶する
    if (CurrentBGMComponent && CurrentBGMComponent->IsPlaying())
    {
        MemorizedFieldBGM = CurrentBGMComponent->Sound;
        CurrentBGMComponent->FadeOut(FadeInTime, 0.0f);
    }

    BGMLoadHandle = UPSAssetManager::Get()->RequestAsyncLoad(InBGM, [this, FadeInTime](UObject* LoadedObject)
        {
            USoundBase* LoadedBGM = Cast<USoundBase>(LoadedObject);
            if (not LoadedBGM)
            {
                DEBUG_MESSAGE_ERROR(TEXT("Failed to cast LoadedObject to USoundBase"));
                UE_LOG(LogTemp, Error, TEXT("Failed to cast LoadedObject to USoundBase")); SET_LOG_PATH(Error);
                return;
            }

            CurrentBGMComponent = UGameplayStatics::CreateSound2D(GetWorld(), LoadedBGM);
            if (CurrentBGMComponent)
            {
                CurrentBGMComponent->FadeIn(FadeInTime, 1.0f);
            }
        });
}

void UPSAudioManager::StopBGM(float FadeOutTime)
{
    if (CurrentBGMComponent && CurrentBGMComponent->IsPlaying())
    {
        CurrentBGMComponent->FadeOut(FadeOutTime, 0.0f);
    }
}

void UPSAudioManager::ResumeFieldBGM(float FadeInTime)
{
    // 現在のボスBGMをフェードアウト
    if (CurrentBGMComponent && CurrentBGMComponent->IsPlaying())
    {
        CurrentBGMComponent->FadeOut(FadeInTime, 0.0f);
    }

    // 記憶していたフィールドBGMがあれば再開
    if (MemorizedFieldBGM)
    {
        CurrentBGMComponent = UGameplayStatics::CreateSound2D(GetWorld(), MemorizedFieldBGM);
        if (CurrentBGMComponent)
        {
            CurrentBGMComponent->FadeIn(FadeInTime, 1.0f);
        }
    }
}

void UPSAudioManager::UpdateCategoryVolume(USoundClass* SoundClassTarget, float NewVolume)
{
    if (not SoundClassTarget)
    {
        DEBUG_MESSAGE_ERROR(TEXT("SoundClassTarget is not valid"));
        UE_LOG(LogTemp, Error, TEXT("SoundClassTarget is not valid")); SET_LOG_PATH(Error);
        return;
    }

    // 特定のSoundClassの音量を上書き
    UGameplayStatics::SetSoundMixClassOverride(
        GetWorld(),
        MainSoundMix,
        SoundClassTarget,
        NewVolume,  // 新しい音量の割合
        1.0f,
        0.5f,
        true
    );
}

void UPSAudioManager::UpdateMasterVolume(float NewVolume)
{
    if (not MasterSoundClass)
    {
        DEBUG_MESSAGE_ERROR(TEXT("MasterSoundClass is not valid"));
        UE_LOG(LogTemp, Error, TEXT("MasterSoundClass is not valid")); SET_LOG_PATH(Error);
        return;
    }

    UpdateCategoryVolume(MasterSoundClass, NewVolume);
}

void UPSAudioManager::UpdateBGMVolume(float NewVolume)
{
    if (not BGMSoundClass)
    {
        DEBUG_MESSAGE_ERROR(TEXT("BGMSoundClass is not valid"));
        UE_LOG(LogTemp, Error, TEXT("BGMSoundClass is not valid")); SET_LOG_PATH(Error);
        return;
    }

    UpdateCategoryVolume(BGMSoundClass, NewVolume);
}

void UPSAudioManager::UpdateSFXVolume(float NewVolume)
{
    if (not SFXSoundClass)
    {
        DEBUG_MESSAGE_ERROR(TEXT("SFXSoundClass is not valid"));
        UE_LOG(LogTemp, Error, TEXT("SFXSoundClass is not valid")); SET_LOG_PATH(Error);
        return;
    }

    UpdateCategoryVolume(SFXSoundClass, NewVolume);
}