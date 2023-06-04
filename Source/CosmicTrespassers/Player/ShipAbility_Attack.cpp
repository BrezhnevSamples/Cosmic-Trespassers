// Copyright Alexander Terentyev. All Rights Reserved.

#include "ShipAbility_Attack.h"
#include "CosmicTrespassersCharacter.h"
#include "Weapons/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

void UShipAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,	const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	const auto* CharacterWeaponUser = Cast<IWeaponUserInterface>(ActorInfo->OwnerActor);	
	CharacterWeaponUser->GetWeaponComponent()->FireProjectile(CharacterWeaponUser->GetProjectileOriginTransform());

	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}