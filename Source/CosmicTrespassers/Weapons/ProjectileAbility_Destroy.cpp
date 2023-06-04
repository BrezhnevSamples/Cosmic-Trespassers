// Copyright Alexander Terentyev. All Rights Reserved.

#include "ProjectileAbility_Destroy.h"
#include "Projectile.h"
#include "Effects/ExplosionEffect.h"

void UProjectileAbility_Destroy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	AActor* Owner = ActorInfo->OwnerActor.Get();
	Cast<IDestroyableInterface>(Owner)->ScheduleDestroy();
	
	UWorld* World = GetWorld();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(FQuat::Identity);

	if (Cast<AProjectile>(Owner)->GetPlayExplosionEffect())
	{
		// Someone's too lazy to make a separate explosion effect
		constexpr float ProjectileEffectScale = 0.3;

		SpawnTransform.SetScale3D(FVector::One() * ProjectileEffectScale);	
		World->SpawnActor(ExplosionEffect, &SpawnTransform);
	}	
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}