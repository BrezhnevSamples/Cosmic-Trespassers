// Copyright Alexander Terentyev. All Rights Reserved.

#include "MessageBlueprintFunctionLibrary.h"
#include "FGamePhaseMessage.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Tags/CosmicTrespassersNativeTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(const FGameplayTag GamePhaseEventTag,	const FGameplayTag GamePhaseTag)
{
	FGamePhaseMessage Message;
	Message.GamePhaseEventTag = GamePhaseEventTag;
	Message.GamePhaseTag = GamePhaseTag;	
	
	UGameplayMessageSubsystem::Get(UCosmicTrespassersGameInstance::Get().GetWorld()).BroadcastMessage(TAG_Message_Channel_Game_Phase, Message);
}
