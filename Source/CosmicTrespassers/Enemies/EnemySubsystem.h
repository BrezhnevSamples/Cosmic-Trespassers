// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FEnemyGrid.h"
#include "FEnemyStack.h"
#include "FEnemyAttackSettings.h"
#include "FEnemyGridSettings.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemySubsystem.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UEnemySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//~=============================================================================
	// Game State
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
    
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag);
    
	//~=============================================================================
	// Tags
private:
	FEnemyGridSettings GridData;
	FEnemyGrid Grid;
    
	//~=============================================================================
	// Enemies
private:
	UPROPERTY()
	TObjectPtr<AActor> EnemyHolder;
	
	float TimeBetweenRowSpawns = 0.5;
	UPROPERTY()
	TArray<TSubclassOf<AEnemy>> EnemyClassPtrs;
	int8 MinAllowedSpawnElevation = -5;
	FVector BonusEnemySpawnLocation = FVector(0, 1500, 1100);
	TArray<float> DestroyedEnemySpeedIncreaseThresholds;
	uint8 CurrentSpawnedRowIndex = 0;
	uint8 RemainingEnemyCount;
	int8 CurrentSpawnElevation = 1;
	int8 CurrentEnemySpeedIncreaseThresholdIndex = 0;
    
	FTimerHandle EnemySpawnTimerHandle;
    
	void ReduceSpawnElevation();
    	
	void SpawnEnemies();
	void OnEnemiesSpawned();
	void ClearGrid();

	UFUNCTION()
	void OnEnemyDied(AEnemy* Enemy);
    	
	//~=============================================================================
	// Bonus Enemy
private:
	UPROPERTY()
	TSoftClassPtr<AActor> BonusEnemyActorClassPtr;
	
	FFloatInterval BonusEnemySpawnInterval;
	
	UPROPERTY()
	TSubclassOf<class UGameplayEffect> InvertEnemySpeedEffect;	
	UPROPERTY()
	TObjectPtr<AEnemy> CurrentBonusEnemy;
    	
	FTimerHandle BonusEnemySpawnTimerHandle;
	FSoftObjectPath BonusEnemyObjectPath;
    
	void SetBonusEnemyTimer();
	void SpawnBonusEnemy();
	void SpawnBonusEnemyDeferred();
	void DestroyBonusEnemy();

	//~=============================================================================
	// Movement
public:
	void AddEnemy(AEnemy* Enemy);

	UFUNCTION()
	void RemoveEnemy(AEnemy* Enemy);
	void StartMoving();
	void StopMoving();
	UFUNCTION()
	void OnLevelBoundReached();
    	
	void ResetMoveDelayIndex();
	void SetMoveDelayIndex(uint8 Index);
	void IncreaseMoveDelayIndex();
    
	float GetVerticalMoveDistance() const { return VerticalMoveDistance; }
    		
private:
	float HorizontalMoveDistance = 20;    
	float VerticalMoveDistance = 50;    	
	TArray<float> MoveDelays;
	UPROPERTY()
	TArray<TObjectPtr<AEnemy>> Enemies;
	uint8 CurrentMoveDelayIndex = 0;
	float CurrentMoveDirection = 1;
	bool bNeedsToMoveDown = false;
	FTimerHandle MoveTimerHandle;
	UPROPERTY()
	TArray<TSoftObjectPtr<USoundWave>> MoveSounds;
	uint8 CurrentMoveSoundIndex = 0;
    
	void MoveEnemies();
	void MoveDown(const float MoveDistance) const;
	void SetMoveTimer();
	void PlayNextMoveSound();
	
	//~=============================================================================
	// Attack
public:
	void ClearStacks();
	void AddEnemyToStack(const uint8 StackIndex, AEnemy* Enemy);
        
	void RemoveEnemyFromStack(AEnemy* Enemy);
	void StartAttacking();
	void StopAttacking();
        	
private:
	UPROPERTY()
	TMap<uint8, FEnemyStack> EnemyStacks;        
	FFloatInterval AttackTimerInterval;
	TArray<FEnemyAttackSettings> AttackDataArray;
	FTimerHandle AttackTimerHandle;
	float TotalAttackWeight;
        
	FEnemyStack GetRandomEnemyStack();
	void ScheduleAttack();
	void PerformAttack();
	FEnemyAttackSettings GetRandomAttackData();        
};