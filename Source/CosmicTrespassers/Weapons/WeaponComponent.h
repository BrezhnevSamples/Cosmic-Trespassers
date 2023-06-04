// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Projectile.h"
#include "WeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRESPASSERS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	UWeaponComponent();

	//~=============================================================================
	// Tags
protected:
	//~=============================================================================
	// Fire
public:
	void SetProjectileClass(const TSubclassOf<AProjectile> NewProjectileClass);
	void FireProjectile(const FTransform& ProjectileOriginTransform) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;
};