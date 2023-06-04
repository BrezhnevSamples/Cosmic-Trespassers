// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FWeightSelectedObject.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FWeightSelectedObject
{
	GENERATED_BODY()
	
public:
	FWeightSelectedObject(): Weight(0), RangeFrom(0), RangeTo(0)	{	}

	FWeightSelectedObject(const FWeightSelectedObject& Other) { *this = Other; }

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;

	UPROPERTY(VisibleAnywhere)
	float RangeFrom;

	UPROPERTY(VisibleAnywhere)
	float RangeTo;	
};