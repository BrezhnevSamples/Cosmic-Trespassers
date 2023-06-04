// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ShipAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COSMICTRESPASSERS_API UShipAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	//~=============================================================================
	// Attributes	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData ReloadTime;
	ATTRIBUTE_ACCESSORS(UShipAttributeSet, ReloadTime)	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData CurrentLevel;
	ATTRIBUTE_ACCESSORS(UShipAttributeSet, CurrentLevel)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxLevel;
	ATTRIBUTE_ACCESSORS(UShipAttributeSet, MaxLevel)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
