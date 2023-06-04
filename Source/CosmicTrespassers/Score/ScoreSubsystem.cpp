// Copyright Alexander Terentyev. All Rights Reserved.

#include "ScoreSubsystem.h"
#include "Enemies/Enemy.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "SaveSystem/SaveSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	CurrentScore = 0;	
	AEnemy::OnEnemyDied.AddDynamic(this, &ThisClass::AddScore);
}

void UScoreSubsystem::RegisterMessageListeners()
{
	GetGameInstance()->GetSubsystem<UGameplayMessageSubsystem>()->RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);
}

void UScoreSubsystem::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
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

void UScoreSubsystem::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Splash))
	{
		HiScore = GetGameInstance()->GetSubsystem<USaveSubsystem>()->GetHiScore();	
	}	
}

void UScoreSubsystem::OnGamePhaseExited(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		CurrentScore = 0;
	}
}

TTuple<uint32, bool> UScoreSubsystem::GetScoreData()
{
	bool bNewHiScoreSet = false;
	
	const USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();

	if (const uint32 OldHiScore = SaveSubsystem->GetHiScore(); CurrentScore > OldHiScore)
	{
		bNewHiScoreSet = true;
		HiScore = CurrentScore;
	}

	return TTuple<uint32, bool>(CurrentScore, bNewHiScoreSet);
}

void UScoreSubsystem::AddScore(AEnemy* DestroyedEnemy)
{
	const uint32 AddedScore = DestroyedEnemy->GetDestroyScore(); 
	CurrentScore += AddedScore;
	ScoreChanged.Broadcast(AddedScore, CurrentScore);
}