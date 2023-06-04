// Copyright Alexander Terentyev. All Rights Reserved.

#include "FEnemyGridCell.h"

bool FEnemyGridCell::IsOccupied() const
{	
	const AEnemy* EnemyPtr = Enemy.Get();

	if (EnemyPtr == nullptr)
	{
		return false;
	}

	return !EnemyPtr->IsScheduledForDestroy() && !EnemyPtr->IsActorBeingDestroyed();
}
