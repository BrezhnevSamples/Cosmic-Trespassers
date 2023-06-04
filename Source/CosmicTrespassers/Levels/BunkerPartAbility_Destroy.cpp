// Copyright Alexander Terentyev. All Rights Reserved.

#include "BunkerPartAbility_Destroy.h"
#include "BunkerPart.h"

void UBunkerPartAbility_Destroy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}	
	
	Cast<ABunkerPart>(ActorInfo->OwnerActor)->Disable();	
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}