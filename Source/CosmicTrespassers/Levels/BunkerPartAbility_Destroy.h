// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "BunkerPartAbility_Destroy.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UBunkerPartAbility_Destroy : public UGameplayAbility
{
	GENERATED_BODY()

	//~=============================================================================
	// Activation	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	
};