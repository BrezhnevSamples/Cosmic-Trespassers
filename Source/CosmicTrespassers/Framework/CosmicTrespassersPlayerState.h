// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "CosmicTrespassersPlayerState.generated.h"

UCLASS()
class COSMICTRESPASSERS_API ACosmicTrespassersPlayerState : public APlayerState
{
	GENERATED_BODY()

	//~=============================================================================
	// State
public:
	void ResetShotCounters();
	void IncreaseTotalShotCounter();
	void IncreaseHitShotCounter();
	void IncreaseMissedShotCounter();
	void DecreaseTotalShotCounter();	

	int32 CalculateAccuracy() const;

private:
	uint32 TotalShots;
	uint32 HitShots;
	uint32 MissedShots;
};