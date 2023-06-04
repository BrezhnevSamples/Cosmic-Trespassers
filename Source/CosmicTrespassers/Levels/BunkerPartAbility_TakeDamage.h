// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "BunkerPartAbility_TakeDamage.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UBunkerPartAbility_TakeDamage : public UGameplayAbility
{
	GENERATED_BODY()

	//~=============================================================================
	// Activation	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};