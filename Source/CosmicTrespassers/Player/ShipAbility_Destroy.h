// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ShipAbility_Destroy.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UShipAbility_Destroy : public UGameplayAbility
{
	GENERATED_BODY()

	//~=============================================================================
	// Activation
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//~=============================================================================
	// Effects
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundWave> ExplosionSound;
};