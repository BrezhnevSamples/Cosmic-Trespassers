// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelLoadSubsystem.generated.h"

UCLASS()
class COSMICTRESPASSERS_API ULevelLoadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	//~=============================================================================
	// Levels
public:
	void LoadGameLevel();
	
private:
	UPROPERTY()
	TSoftObjectPtr<UWorld> GameLevelPtr;
	
	UFUNCTION()
	void OnGameLevelLoaded();
};