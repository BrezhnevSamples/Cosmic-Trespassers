// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CommonAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COSMICTRESPASSERS_API UCommonAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	//~=============================================================================
	// Attributes	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UCommonAttributeSet, CurrentHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCommonAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCommonAttributeSet, MoveSpeed)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	bool HealthIsMax() const { return GetCurrentHealth() == GetMaxHealth(); }
};