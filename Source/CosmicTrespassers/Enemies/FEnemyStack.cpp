// Copyright Alexander Terentyev. All Rights Reserved.

#include "FEnemyStack.h"

void FEnemyStack::AddEnemy(AEnemy* Enemy)
{
	Enemies.Emplace(TObjectPtr<AEnemy>(Enemy));
}

void FEnemyStack::RemoveEnemy(AEnemy* Enemy, bool& bStackIsEmptyOut)
{
	Enemies.Remove(Enemy);
	bStackIsEmptyOut = Enemies.IsEmpty();
}

const AEnemy* FEnemyStack::GetFirstEnemy()
{
	if (Enemies.IsEmpty())
	{
		return nullptr;
	}
	
	return Enemies[0].Get();
}

bool FEnemyStack::Contains(AEnemy* Enemy) const
{
	return Enemies.Contains(Enemy);	
}
