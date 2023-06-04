// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FEnemyGridCell.h"
#include "FEnemyGridSettings.h"
#include "Utility/Array2D.h"
#include "FEnemyGrid.generated.h"

using FVector2Uint8 = UE::Math::TIntVector2<uint8>;

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FEnemyGrid
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:	
	explicit FEnemyGrid(): Cells(0, 0), LeftmostColumnIndex(0), RightmostColumnIndex(0) {	}
	FEnemyGrid(const FEnemyGridSettings& GridSettings);
		

	//~=============================================================================
	// Cells
public:
	TArray2D<FEnemyGridCell> Cells;
	
private:	
	TArray<uint8> ActiveColumnIndices;
	uint8 LeftmostColumnIndex;
	uint8 RightmostColumnIndex;
};