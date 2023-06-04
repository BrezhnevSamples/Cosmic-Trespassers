// Copyright Alexander Terentyev. All Rights Reserved.

#include "LevelBound.h"
#include "GameplayTagAssetInterface.h"
#include "Components/BoxComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/MessageBlueprintFunctionLibrary.h"

FOnLevelBoundReached ALevelBound::OnLevelBoundReached;

ALevelBound::ALevelBound()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");	
	BoxComponent->SetupAttachment(RootComponent);
}

void ALevelBound::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);

	OnActorBeginOverlap.AddDynamic(this, &ALevelBound::OnBeginOverlap);
}

void ALevelBound::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
}

void ALevelBound::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Gameplay))
	{
		EnableCollisions();
	}
}

void ALevelBound::OnBeginOverlap(AActor*, AActor* OtherActor)
{
	if (bIgnoreCollisions)
	{
		return;

	}
	
	if (Type == ELevelBoundType::Bottom)
	{
		UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_Game_Over);		
	}
	else
	{
		if (const auto* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(OtherActor);
			GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Enemy_Bonus) || GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Ship))
		{
			return;
		}
	
		bIgnoreCollisions = true;
	
		OnLevelBoundReached.ExecuteIfBound();
	}	
	constexpr float DelayBeforeEnablingCollisions = 2;
	GetWorldTimerManager().SetTimer(CollisionEnableTimerHandle, FTimerDelegate::CreateUObject(this, &ALevelBound::EnableCollisions),DelayBeforeEnablingCollisions, false);	
}

void ALevelBound::EnableCollisions()
{
	bIgnoreCollisions = false;
	GetWorldTimerManager().ClearTimer(CollisionEnableTimerHandle);
}