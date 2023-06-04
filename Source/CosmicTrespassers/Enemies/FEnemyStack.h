// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Enemy.h"
#include "FEnemyStack.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FEnemyStack
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:
	FEnemyStack() {}
	FEnemyStack(const FEnemyStack& Other) { *this = Other; }

	//~=============================================================================
	// Enemies
public:
	void AddEnemy(AEnemy* Enemy);	
	void RemoveEnemy(AEnemy* Enemy, bool& bStackIsEmptyOut);
	const AEnemy* GetFirstEnemy();

	bool Contains(AEnemy* Enemy) const;	
	
private:	
	TArray<TObjectPtr<AEnemy>> Enemies;	
};