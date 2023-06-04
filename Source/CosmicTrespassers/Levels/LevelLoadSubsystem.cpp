// Copyright Alexander Terentyev. All Rights Reserved.

#include "LevelLoadSubsystem.h"
#include "LevelLoadSubsystemSettings.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Messages/MessageBlueprintFunctionLibrary.h"
#include "Tags/CosmicTrespassersNativeTags.h"

void ULevelLoadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GameLevelPtr = GetDefault<ULevelLoadSubsystemSettings>()->GameLevelPtr;
}

void ULevelLoadSubsystem::LoadGameLevel()
{	
	FLatentActionInfo LoadCallback;
	LoadCallback.CallbackTarget = this;
	LoadCallback.Linkage = 0;
	LoadCallback.UUID = 0;
	LoadCallback.ExecutionFunction = "OnGameLevelLoaded";
	
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), GameLevelPtr, true, false, LoadCallback);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ULevelLoadSubsystem::OnGameLevelLoaded()
{	
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_StartMenu);
}