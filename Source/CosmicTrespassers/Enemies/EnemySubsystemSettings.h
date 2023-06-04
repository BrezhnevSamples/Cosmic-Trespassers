// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FEnemyGrid.h"
#include "FEnemyGridSettings.h"
#include "FEnemyAttackSettings.h"
#include "Engine/DeveloperSettings.h"
#include "EnemySubsystemSettings.generated.h"

UCLASS(Config=Game, Defaultconfig, meta=(DisplayName="Enemy Settings"))
class COSMICTRESPASSERS_API UEnemySubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
                    
	//~=============================================================================
	// Grid
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	FEnemyGridSettings GridData;
                    
	//~=============================================================================
	// Enemies
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	float TimeBetweenRowSpawns = 0.5;
                    	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	TArray<TSubclassOf<AEnemy>> EnemyClassPtrs;
                    
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	int MinAllowedSpawnElevation = -5;
                    
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	TArray<float> DestroyedEnemySpeedIncreaseThresholds;
                    	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Bonus Enemy")
	TSubclassOf<class UGameplayEffect> InvertEnemySpeedEffect;
	
	//~=============================================================================S
	// Bonus Enemy
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Bonus Enemy")
	TSoftClassPtr<AActor> BonusEnemyActorClassPtr;
                    	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Bonus Enemy")
	FFloatInterval BonusEnemySpawnInterval;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	FVector BonusEnemySpawnLocation = FVector(0, 1500, 1100);
	
	//~=============================================================================
	// Movement
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float HorizontalMoveDistance = 20;
    
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")	
	float VerticalMoveDistance = 50;
    	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TArray<float> MoveDelays;
    	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TArray<TSoftObjectPtr<USoundWave>> MoveSounds;
	
	//~=============================================================================
	// Attack
public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FFloatInterval AttackTimerInterval;
    
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TArray<FEnemyAttackSettings> AttackDataArray;
};