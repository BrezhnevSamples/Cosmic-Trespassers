// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ProjectileAbility_Destroy.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UProjectileAbility_Destroy : public UGameplayAbility
{
	GENERATED_BODY()	

	//~=============================================================================
	// Activation
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//~=============================================================================
	// EffectsS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AExplosionEffect> ExplosionEffect;
};