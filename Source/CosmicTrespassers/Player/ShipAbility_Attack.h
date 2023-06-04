// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ShipAbility_Attack.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UShipAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()

	//~=============================================================================
	// Settings
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundWave> FireSound;
	
	//~=============================================================================
	// Activation	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};