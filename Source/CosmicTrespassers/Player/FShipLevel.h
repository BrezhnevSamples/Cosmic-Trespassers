// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FShipAttribute.h"
#include "FShipLevel.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FShipLevel
{
	GENERATED_BODY()

	//~=============================================================================
	//
public:
	FShipLevel(): ScoreToReach(0) { }

	//~=============================================================================
	// Data	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ScoreToReach;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UPaperSprite> SpriteObjectPtr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class AProjectile> ProjectileClassPtr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FShipAttribute> ShipAttributeDataArray;
};
