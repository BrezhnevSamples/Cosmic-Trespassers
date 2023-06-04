// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FBunkerRowSpawnData.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FBunkerRowSpawnData
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	FBunkerRowSpawnData() {}

	//~=============================================================================
	// Spawn Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInt32Interval> PartColumnIntervals;
};
