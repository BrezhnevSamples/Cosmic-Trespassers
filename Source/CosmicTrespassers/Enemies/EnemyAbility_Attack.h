// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EnemyAbility_Attack.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UEnemyAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()

	//~=============================================================================
	// Activation
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	
};