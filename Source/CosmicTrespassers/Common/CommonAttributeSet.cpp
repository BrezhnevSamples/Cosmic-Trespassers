// Copyright Alexander Terentyev. All Rights Reserved.

#include "CommonAttributeSet.h"
#include "Engine/Engine.h"

void UCommonAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetCurrentHealthAttribute() && GetMaxHealth() != 0)
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());		
	}
}