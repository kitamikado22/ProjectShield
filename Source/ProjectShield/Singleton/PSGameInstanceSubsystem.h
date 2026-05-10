// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/PSDebugMessageMacross.h"
#include "PSGameInstanceSubsystem.generated.h"

/**
 * プロジェクト内の全てのシングルトンクラスの基底クラス
 */
UCLASS(Abstract, Blueprintable)
class PROJECTSHIELD_API UPSGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	/** インスタンスを取得する */
    template <typename T>
    static T* Get(const UObject* WorldContextObject)
    {
        if (not WorldContextObject)
        {
            return nullptr;
        }

        UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
        if (not GameInstance)
        {
            return nullptr;
        }

        // 全てのGameInstanceSubsystemを取得して検索
        const TArray<UPSGameInstanceSubsystem*>& Subsystems = GameInstance->GetSubsystemArrayCopy<UPSGameInstanceSubsystem>();
        for (UGameInstanceSubsystem* Subsystem : Subsystems)
        {
            // T（C++クラス）を継承しているインスタンス（BP含む）を見つけたらキャストして返す
            if (Subsystem && Subsystem->IsA<T>())
            {
                return Cast<T>(Subsystem);
            }
        }

        DEBUG_MESSAGE_WARNING(TEXT("The subsystem for the specified class could not be found."));
        UE_LOG(LogTemp, Log, TEXT("The subsystem for the specified class could not be found."));

        return nullptr;
    }

protected:

    /** システム側が抽象クラスを実体化しないように設定する */
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override
    {
        if (!Super::ShouldCreateSubsystem(Outer)) return false;

        // 抽象クラスではないものがtrueとなり、インスタンスを持つ
        // この基底クラスもインスタンス禁止
        return not GetClass()->HasAnyClassFlags(CLASS_Abstract) && GetClass() != UPSGameInstanceSubsystem::StaticClass();
    }
};
