// Copyright Alexander Terentyev. All Rights Reserved.

#include "EnemyAbility_Destroy.h"
#include "AbilitySystemComponent.h"
#include "Common/DestroyableInterface.h"
#include "Effects/ExplosionEffect.h"
#include "Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAbility_Destroy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,	const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	AActor* Owner = ActorInfo->OwnerActor.Get();
	
	Owner->SetActorHiddenInGame(true);
	Cast<IDestroyableInterface>(Owner)->ScheduleDestroy();

	Owner->SetActorEnableCollision(false);

	UWorld* World = GetWorld();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(FQuat::Identity);
	
	World->SpawnActor(ExplosionEffect, &SpawnTransform);
	UGameplayStatics::PlaySound2D(World, ExplosionSound);
	
	AEnemy::OnEnemyDied.Broadcast(Cast<AEnemy>(Owner));	

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}