// Copyright Alexander Terentyev. All Rights Reserved.

#include "WeaponComponent.h"
#include "Projectile.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Framework/CosmicTrespassersPlayerState.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::SetProjectileClass(const TSubclassOf<AProjectile> NewProjectileClass)
{
	ProjectileClass = NewProjectileClass;
}

void UWeaponComponent::FireProjectile(const FTransform& ProjectileOriginTransform) const
{
	AActor* Owner = GetOwner();

	if (UCosmicTrespassersGameInstance& GameInstance = UCosmicTrespassersGameInstance::Get(); GameInstance.IsPlayerCharacter(Owner))
	{
		GameInstance.GetPlayerState<ACosmicTrespassersPlayerState>()->IncreaseTotalShotCounter();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Cast<APawn>(Owner);
	
	AProjectile* Projectile = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileClass, &ProjectileOriginTransform, SpawnParameters));

	const FGameplayTag OwnerTag = UCosmicTrespassersGameInstance::Get().IsPlayerCharacter(GetOwner()) ? TAG_Ship : TAG_Enemy;	
	Projectile->SetOwnerTag(OwnerTag);
}