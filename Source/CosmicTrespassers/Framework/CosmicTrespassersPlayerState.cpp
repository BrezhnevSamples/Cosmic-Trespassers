// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersPlayerState.h"

void ACosmicTrespassersPlayerState::ResetShotCounters()
{
	TotalShots = 0;
	HitShots = 0;
	MissedShots = 0;	
}

void ACosmicTrespassersPlayerState::IncreaseTotalShotCounter()
{
	TotalShots++;
}

void ACosmicTrespassersPlayerState::IncreaseHitShotCounter()
{
	HitShots++;
}

void ACosmicTrespassersPlayerState::IncreaseMissedShotCounter()
{
	MissedShots++;
}

void ACosmicTrespassersPlayerState::DecreaseTotalShotCounter()
{
	if (TotalShots == 0)
	{
		return;
	}
	
	TotalShots--;
}

int32 ACosmicTrespassersPlayerState::CalculateAccuracy() const
{
	if (TotalShots == 0)
	{
		return 0;
	}	
	
	return FMath::TruncToInt(static_cast<float>(HitShots) / static_cast<float>(TotalShots) * 100);
}