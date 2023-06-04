// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Utility/FWeightSelectedObject.h"
#include "FBunkerDamagePattern.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FBunkerDamagePattern : public FWeightSelectedObject
{
	GENERATED_BODY()

	//~=============================================================================
	// Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVector> DamageOffsets;
};