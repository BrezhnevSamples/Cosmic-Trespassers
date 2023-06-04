// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FEnemyGridSettings.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FEnemyGridSettings
{
	GENERATED_BODY()
	
	//~=============================================================================
	// Data
public:
	FEnemyGridSettings() {}
	FEnemyGridSettings(const FEnemyGridSettings& Other) { *this = Other; }
	
	//~=============================================================================
	// Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 RowCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 ColumnCount = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceBetweenRows = 120;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceBetweenColumns = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GridStartZ = 600;

	uint8 GetTotalNumberOfCells() const { return RowCount * ColumnCount; }
};