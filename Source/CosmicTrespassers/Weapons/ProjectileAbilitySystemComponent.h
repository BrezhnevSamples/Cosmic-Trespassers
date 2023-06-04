// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "ProjectileAbilitySystemComponent.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UProjectileAbilitySystemComponent : public UCosmicTrespassersAbilitySystemComponent
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	virtual void Init() override;
};