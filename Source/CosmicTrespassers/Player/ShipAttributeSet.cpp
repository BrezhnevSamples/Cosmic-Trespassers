// Copyright Alexander Terentyev. All Rights Reserved.

#include "ShipAttributeSet.h"

void UShipAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetCurrentLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1, GetMaxLevel());
	}
}