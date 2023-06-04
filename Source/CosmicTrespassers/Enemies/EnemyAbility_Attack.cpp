// Copyright Alexander Terentyev. All Rights Reserved.

#include "EnemyAbility_Attack.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "Weapons/WeaponComponent.h"
#include "Weapons/WeaponUserInterface.h"

struct FGameplayEventData;

void UEnemyAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,	const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	const auto* EnemyWeaponUser = Cast<IWeaponUserInterface>(ActorInfo->OwnerActor);	
	EnemyWeaponUser->GetWeaponComponent()->FireProjectile(EnemyWeaponUser->GetProjectileOriginTransform());

	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
}