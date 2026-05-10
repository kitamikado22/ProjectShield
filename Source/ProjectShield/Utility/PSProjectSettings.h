// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PSProjectSettings.generated.h"

/**
 * プロジェクト内の定数や参照をもつデータコンテナ的な
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Project Shield Settings"))
class PROJECTSHIELD_API UPSProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

    UPSProjectSettings()
    {
        CategoryName = TEXT("Game");
        SectionName = TEXT("ProjectShield");
    }

    static const UPSProjectSettings* Get() { return GetDefault<UPSProjectSettings>(); }

    /** BPで作成したサブシステムなどは参照などをしていないとインスタンス化しない問題があるため、その参照用 */
    UPROPERTY(Config, EditAnywhere, Category = "Subsystems", meta = (AllowAbstract = "false"))
    TArray<TSubclassOf<class UPSGameInstanceSubsystem>> SubsystemClasses;

    

};
