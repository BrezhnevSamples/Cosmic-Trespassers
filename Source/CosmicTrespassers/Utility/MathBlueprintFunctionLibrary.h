// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathBlueprintFunctionLibrary.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UMathBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//~=============================================================================
	// Random
public:
	static float GetRandomFloatFromInterval(const FFloatInterval Interval);
};