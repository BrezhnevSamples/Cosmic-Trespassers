// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Messages/FGamePhaseMessage.h"
#include "GameFramework/Actor.h"
#include "ScoreSubsystem.generated.h"

DECLARE_EVENT_TwoParams(UScoreSubsystem, FOnScoreChanged, uint32, uint32)

UCLASS()
class COSMICTRESPASSERS_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterMessageListeners();

private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag);
	
	//~=============================================================================
	// Score
public:	
	FOnScoreChanged& OnScoreChanged() { return ScoreChanged; }
	uint32 GetCurrentScore() const { return CurrentScore; }
	uint32 GetHiScore() const { return HiScore; }

	TTuple<uint32, bool> GetScoreData();

private:
	uint32 CurrentScore;
	uint32 HiScore;
	
	FOnScoreChanged ScoreChanged;

	UFUNCTION()
	void AddScore(class AEnemy* DestroyedEnemy);	
};