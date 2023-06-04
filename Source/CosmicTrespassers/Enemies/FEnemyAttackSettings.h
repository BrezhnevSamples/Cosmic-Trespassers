// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Utility/FWeightSelectedObject.h"
#include "FEnemyAttackSettings.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FEnemyAttackSettings : public FWeightSelectedObject
{	
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	FEnemyAttackSettings(): NumberOfAttacks(1)	{}

	FEnemyAttackSettings(const FEnemyAttackSettings& Other) { *this = Other; }

	//~=============================================================================
	// Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 NumberOfAttacks;
};