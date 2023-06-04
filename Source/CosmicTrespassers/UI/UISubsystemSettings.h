// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "UISubsystemSettings.generated.h"

UCLASS(Config=Game, Defaultconfig, meta=(DisplayName="UI Settings"))
class COSMICTRESPASSERS_API UUISubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	//~=============================================================================
	// Widgets
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USplashWidget> SplashWidget;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UStartMenuWidget> StartMenuWidget;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UHUDWidget> HUDWidget;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameOverWidget> GameOverWidget;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> PauseMenuWidget;	
};