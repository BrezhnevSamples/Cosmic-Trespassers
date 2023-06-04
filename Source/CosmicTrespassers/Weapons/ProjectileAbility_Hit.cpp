// Copyright Alexander Terentyev. All Rights Reserved.

#include "ProjectileAbility_Hit.h"
#include "AbilitySystemInterface.h"
#include "Projectile.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Framework/CosmicTrespassersPlayerState.h"

void UProjectileAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	AActor* Owner = ActorInfo->OwnerActor.Get();
	Owner->SetActorEnableCollision(false);

	FGameplayTagContainer OwnerTagContainer;				
	Cast<IGameplayTagAssetInterface>(Owner)->GetOwnedGameplayTags(OwnerTagContainer);
	
	AActor* OtherActor = Cast<AActor>(TriggerEventData->OptionalObject);

	if (const auto* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor); AbilitySystemInterface != nullptr)
	{
		if (const auto* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(AbilitySystemInterface); GameplayTagAssetInterface != nullptr)
		{			
			auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
			
			if (GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Bunker))
			{
				// If we hit a bunker part, we will destroy it but also trigger an ability that applies a damage pattern to nearby parts
				
				FGameplayEventData GameplayEventData;
				GameplayEventData.Instigator = Owner;

				AbilitySystem->HandleGameplayEvent(TAG_Bunker_Ability_TakeDamage_Trigger, &GameplayEventData);
				
				if (OwnerTagContainer.HasTagExact(TAG_Ship))
				{
					UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->IncreaseMissedShotCounter();
				}
			}
			else
			{
				if (OwnerTagContainer.HasTagExact(TAG_Ship))
				{
					UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->IncreaseHitShotCounter();
				}
			}
			
			AbilitySystem->ApplyEffectToSelf(DamageEffect, 1);

			if (GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Bunker))
			{
				Cast<AProjectile>(Owner)->SetPlayExplosionEffect(true);	
			}
			else if (GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Projectile) && !Cast<AProjectile>(OtherActor)->GetPlayExplosionEffect())
			{
				Cast<AProjectile>(Owner)->SetPlayExplosionEffect(true);	
			}
		}
		else
		{
			if (OwnerTagContainer.HasTagExact(TAG_Ship))
			{
				UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->IncreaseMissedShotCounter();
			}

			Cast<AProjectile>(Owner)->SetPlayExplosionEffect(true);	
		}	
	}
	else
	{
		Cast<AProjectile>(Owner)->SetPlayExplosionEffect(true);	
		
		if (OwnerTagContainer.HasTagExact(TAG_Ship))
		{
			UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->IncreaseMissedShotCounter();
		}
	}

	Cast<UCosmicTrespassersAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo())->ApplyEffectToSelf(DamageEffect, 1);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
