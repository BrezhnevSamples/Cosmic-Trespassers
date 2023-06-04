// Copyright Alexander Terentyev. All Rights Reserved.

#include "UISubsystem.h"
#include "HUDWidget.h"
#include "SplashWidget.h"
#include "GameOverWidget.h"
#include "StartMenuWidget.h"
#include "UISubsystemSettings.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	auto* Settings = GetDefault<UUISubsystemSettings>();

	SplashWidget = Settings->SplashWidget;
	StartMenuWidget = Settings->StartMenuWidget;
	HUDWidget = Settings->HUDWidget;
	GameOverWidget = Settings->GameOverWidget;
	PauseMenuWidget = Settings->PauseMenuWidget;

	ShowSplashScreen();
}

void UUISubsystem::RegisterMessageListeners()
{
	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);	
}

void UUISubsystem::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
	else if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Exited))
	{
		OnGamePhaseExited(Message.GamePhaseTag);
	}
}

void UUISubsystem::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_StartMenu))
	{
		HideSplashScreen();
		ShowStartMenu();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Gameplay))
	{
		ShowHUD();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		HideHUD();
		ShowGameOver();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Pause))
	{
		ShowPauseMenu();
	}
}

void UUISubsystem::OnGamePhaseExited(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_StartMenu))
	{
		HideStartMenu();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		HideGameOver();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Pause))
	{
		HidePauseMenu();
	}
}

void UUISubsystem::ShowSplashScreen()
{
	if (ActiveSplashWidget != nullptr)
	{
		return;
	}

	ActiveSplashWidget = Cast<USplashWidget>(CreateWidget(GetWorld(), SplashWidget));
	ActiveSplashWidget->AddToViewport();	
}

void UUISubsystem::HideSplashScreen()
{
	if (ActiveSplashWidget == nullptr)
	{
		return;
	}

	ActiveSplashWidget->RemoveFromParent();
	ActiveSplashWidget = nullptr;
}

void UUISubsystem::ShowStartMenu()
{	
	if (ActiveStartMenuWidget != nullptr)
	{
		return;		
	}

	ActiveStartMenuWidget = Cast<UStartMenuWidget>(CreateWidget(GetWorld(), StartMenuWidget));
	ActiveStartMenuWidget->AddToViewport();
}

void UUISubsystem::HideStartMenu()
{	
	if (ActiveStartMenuWidget == nullptr)
	{
		return;
	}
	
	ActiveStartMenuWidget->RemoveFromParent();
	ActiveStartMenuWidget = nullptr;
}

void UUISubsystem::ShowHUD()
{
	if (ActiveHUDWidget != nullptr)
	{
		return;
	}

	ActiveHUDWidget = Cast<UHUDWidget>(CreateWidget(GetWorld(), HUDWidget));
	ActiveHUDWidget->AddToViewport();
}

void UUISubsystem::HideHUD()
{
	if (ActiveHUDWidget == nullptr)
	{
		return;
	}

	ActiveHUDWidget->RemoveFromParent();
	ActiveHUDWidget = nullptr;
}

void UUISubsystem::ShowGameOver()
{
	if (ActiveGameOverWidget != nullptr)
	{
		return;
	}

	ActiveGameOverWidget = Cast<UGameOverWidget>(CreateWidget(GetWorld(), GameOverWidget));
	ActiveGameOverWidget->AddToViewport();
}

void UUISubsystem::HideGameOver()
{
	if (ActiveGameOverWidget == nullptr)
	{
		return;
	}

	ActiveGameOverWidget->RemoveFromParent();
	ActiveGameOverWidget = nullptr;
}

void UUISubsystem::ShowPauseMenu()
{
	if (ActivePauseMenuWidget != nullptr)
	{
		return;
	}

	ActivePauseMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), PauseMenuWidget));
	ActivePauseMenuWidget->AddToViewport();
}

void UUISubsystem::HidePauseMenu()
{
	if (ActivePauseMenuWidget == nullptr)
	{
		return;
	}

	ActivePauseMenuWidget->RemoveFromParent();
	ActivePauseMenuWidget = nullptr;
}