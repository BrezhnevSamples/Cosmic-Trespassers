// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Enemy.h"
#include "FEnemyGridCell.generated.h"

using FVector2Uint8 = UE::Math::TIntVector2<uint8>;

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FEnemyGridCell
{
	GENERATED_BODY()	

	//~=============================================================================
	// Instance
public:
	FEnemyGridCell() { GridCoordinates = FVector2Uint8(0, 0), RelativeLocation = FVector::Zero(), Enemy = nullptr; }

	FEnemyGridCell(const FVector2Uint8 CellCoordinates, const FVector Location)
	{
		GridCoordinates = CellCoordinates;
		RelativeLocation = Location;
	}

	FEnemyGridCell(const FEnemyGridCell& Other) { *this = Other; }

	//~=============================================================================
	// Data
public:
	void SetEnemy(AEnemy* CellEnemy) { Enemy = TObjectPtr<AEnemy>(CellEnemy); }
	AEnemy* GetEnemy() const { return Enemy.Get(); }
	
	FVector2Uint8 GetGridCoordinates() const { return GridCoordinates; }
	FVector GetLocation() const { return RelativeLocation; }
	bool IsOccupied() const;

private:
	UPROPERTY()
	TObjectPtr<AEnemy> Enemy;
	FVector2Uint8 GridCoordinates;
	FVector RelativeLocation;
};