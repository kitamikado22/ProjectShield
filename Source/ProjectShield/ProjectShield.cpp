// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectShield.h"
#include "GAS/PSGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Modules/ModuleManager.h"

class FProjectShieldModule : public FDefaultGameModuleImpl
{
public:

	virtual void StartupModule() override
	{	

	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectShieldModule, ProjectShield, "ProjectShield");
