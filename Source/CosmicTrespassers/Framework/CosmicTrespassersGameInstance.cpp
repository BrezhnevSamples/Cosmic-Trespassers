// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersGameInstance.h"
#include "CosmicTrespassersPlayerState.h"
#include "GameplayTagAssetInterface.h"
#include "Levels/LevelLoadSubsystem.h"
#include "Messages/MessageBlueprintFunctionLibrary.h"
#include "Score/ScoreSubsystem.h"
#include "Tags/CosmicTrespassersNativeTags.h"
#include "UI/UISubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameUserSettings.h"

TObjectPtr<UCosmicTrespassersGameInstance> UCosmicTrespassersGameInstance::Instance = nullptr;

void UCosmicTrespassersGameInstance::Init()
{
	Instance = this;
	
	Super::Init();	
	
	GetSubsystem<UScoreSubsystem>()->RegisterMessageListeners();
	GetSubsystem<UUISubsystem>()->RegisterMessageListeners();

	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_Splash);
	
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	Settings->SetScreenResolution(FIntPoint(1920, 1080));
	Settings->ApplySettings(false);	
	
	InitIfReady();
}

void UCosmicTrespassersGameInstance::InitIfReady()
{
	if (PlayerController == nullptr || Character == nullptr)
	{
		return;
	}	

	GetSubsystem<UGameplayMessageSubsystem>()->RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);
	GetSubsystem<ULevelLoadSubsystem>()->LoadGameLevel();
}

void UCosmicTrespassersGameInstance::StartGame()
{
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Exited, TAG_Game_Phase_StartMenu);
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage( TAG_Game_Phase_Entered, TAG_Game_Phase_SpawnEnemies);		
}

void UCosmicTrespassersGameInstance::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Exited))
	{
		OnGamePhaseExited(Message.GamePhaseTag);
	}
}

void UCosmicTrespassersGameInstance::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	PreviousGamePhaseTag = CurrentGamePhaseTag;	 
	CurrentGamePhaseTag = GamePhaseTag;
}

void UCosmicTrespassersGameInstance::OnGamePhaseExited(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		GetPlayerState<ACosmicTrespassersPlayerState>()->ResetShotCounters();
	}
}

void UCosmicTrespassersGameInstance::SetCharacter(APawn* CosmicTrespassersCharacter)
{
	Character = CosmicTrespassersCharacter;
	InitIfReady();
}

void UCosmicTrespassersGameInstance::SetPlayerController(APlayerController* CosmicTrespassersPlayerController)
{
	PlayerController = CosmicTrespassersPlayerController;
	InitIfReady();
}

bool UCosmicTrespassersGameInstance::IsPlayerCharacter(AActor* Actor) const
{
	if (Actor == nullptr)
	{
		return false;
	}
	
	const auto* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor);

	if (GameplayTagAssetInterface != nullptr)
	{
		return GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Ship);
	}

	return false;
}

bool UCosmicTrespassersGameInstance::IsEnemy(AActor* Actor) const
{
	if (Actor == nullptr)
	{
		return false;
	}
	
	const auto* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor);

	if (GameplayTagAssetInterface != nullptr)
	{
		return GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Enemy);
	}

	return false;
}

void UCosmicTrespassersGameInstance::TogglePause()
{	
	bPaused = !bPaused;	
	
	if (bPaused)
	{
		GamePhaseBeforePauseTag = CurrentGamePhaseTag;
		UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_Pause);		
	}
	else
	{
		UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, GamePhaseBeforePauseTag);	
	}
	
	UGameplayStatics::SetGamePaused(GetWorld(), bPaused);
}