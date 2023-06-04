// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "WeaponUserInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponUserInterface : public UInterface
{
	GENERATED_BODY()
};

class COSMICTRESPASSERS_API IWeaponUserInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Weapon	
public:
	virtual class UWeaponComponent* GetWeaponComponent() const = 0;
	virtual FTransform GetProjectileOriginTransform() const = 0;
};