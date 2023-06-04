// Copyright Alexander Terentyev. All Rights Reserved.

#include "BunkerPartAbility_TakeDamage.h"
#include "BunkerPart.h"

void UBunkerPartAbility_TakeDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	auto* Owner = Cast<ABunkerPart>(ActorInfo->OwnerActor);
	Owner->OnDamageTaken(Cast<AProjectile>(TriggerEventData->Instigator));
	
	//TODO: Play destroy effect here
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}