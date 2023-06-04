// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "LevelLoadSubsystemSettings.generated.h"

UCLASS(Config=Game, Defaultconfig, meta=(DisplayName="Level Load Settings"))
class COSMICTRESPASSERS_API ULevelLoadSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	//~=============================================================================
	// Settings
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> GameLevelPtr;
};