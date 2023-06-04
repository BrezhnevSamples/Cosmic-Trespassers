// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AttributeSet.h"
#include "FShipAttribute.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FShipAttribute
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	FShipAttribute(): Value(0) { }

	//~=============================================================================
	// Data	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute GameplayAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
};