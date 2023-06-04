// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EnemyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COSMICTRESPASSERS_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	//~=============================================================================
	// Attributes
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData DestroyScore;
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, DestroyScore)	
};