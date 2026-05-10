// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utility/PSAssetManager.h"
#include "Singleton/PSGameInstanceSubsystem.h"
#include "PSAudioManager.generated.h"

class USoundBase;

/**
 * ゲーム中のBGMなどを管理する
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class PROJECTSHIELD_API UPSAudioManager : public UPSGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

    /** メインのサウンドミックス */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Audio")
    TObjectPtr<USoundMix> MainSoundMix;

    /** マスターサウンド */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Audio")
    TObjectPtr<USoundClass> MasterSoundClass;

    /** BGM */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Audio")
    TObjectPtr<USoundClass> BGMSoundClass;

    /** SE SFX */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PS|Audio")
    TObjectPtr<USoundClass> SFXSoundClass;

public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /** 指定のBGMを再生する */
    void PlayEnemyBGM(TSoftObjectPtr<USoundBase> InBGM, float FadeInTime = 2.0f);

    /** 現在再生されているBGMをストップ */
    void StopBGM(float FadeOutTime = 2.0f);

    /** フィールド上の通常のBGMを再生 */
    void ResumeFieldBGM(float FadeInTime = 2.0f);

    /** 特定のカテゴリーの音量を更新する */
    void UpdateCategoryVolume(USoundClass* SoundClassTarget, float NewVolume);

    /** マスター音量を設定 */
    void UpdateMasterVolume(float NewVolume);

    /** BGMの音量を設定 */
    void UpdateBGMVolume(float NewVolume);

    /** SE,SFXの音量を設定 */
    void UpdateSFXVolume(float NewVolume);

private:

    /** 現在再生中のBGMコンポーネントを保持 */
    TObjectPtr<UAudioComponent> CurrentBGMComponent;

    /** ボス戦前に流れていたBGMを記憶しておく用 */
    TObjectPtr<USoundBase> MemorizedFieldBGM;

    /** BGMロードハンドル */
    TSharedPtr<FPSAssetLoadHandle> BGMLoadHandle;
};
