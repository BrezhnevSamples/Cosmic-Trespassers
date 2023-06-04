// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Messages/FGamePhaseMessage.h"
#include "GameFramework/Actor.h"
#include "UISubsystem.generated.h"

class USplashWidget;
class UStartMenuWidget;
class UHUDWidget;
class UGameOverWidget;

UCLASS()
class COSMICTRESPASSERS_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void RegisterMessageListeners();

	//~=============================================================================
	// Game State
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);

	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag);

	//~=============================================================================
	// Splash
private:
	UPROPERTY()
	TSubclassOf<USplashWidget> SplashWidget;

	UPROPERTY()
	TObjectPtr<USplashWidget> ActiveSplashWidget;

	void ShowSplashScreen();
	void HideSplashScreen();
	
	//~=============================================================================
	// Start Menu
private:
	UPROPERTY()
	TSubclassOf<UStartMenuWidget> StartMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UStartMenuWidget> ActiveStartMenuWidget;

	void ShowStartMenu();
	void HideStartMenu();
	
	//~=============================================================================
	// HUD
private:
	UPROPERTY()
	TSubclassOf<UHUDWidget> HUDWidget;
	
	UPROPERTY()
	TObjectPtr<UHUDWidget> ActiveHUDWidget;

	void ShowHUD();
	void HideHUD();	

	//~=============================================================================
	// Game Over
private:
	UPROPERTY()
	TSubclassOf<UGameOverWidget> GameOverWidget;
	
	UPROPERTY()
	TObjectPtr<UGameOverWidget> ActiveGameOverWidget;

	void ShowGameOver();
	void HideGameOver();	

	//~=============================================================================
	// Pause Menu
private:
	UPROPERTY()
	TSubclassOf<UUserWidget> PauseMenuWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActivePauseMenuWidget;

	void ShowPauseMenu();
	void HidePauseMenu();
};