// Copyright Alexander Terentyev. All Rights Reserved.

#include "MathBlueprintFunctionLibrary.h"

float UMathBlueprintFunctionLibrary::GetRandomFloatFromInterval(const FFloatInterval Interval)
{
	return FMath::RandRange(Interval.Min, Interval.Max);
}