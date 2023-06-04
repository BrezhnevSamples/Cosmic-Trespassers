// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FShipLevel.h"
#include "Engine/DataAsset.h"
#include "ShipProgressionConfig.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UShipProgressionConfig : public UDataAsset
{
	GENERATED_BODY()

	//~=============================================================================
	// Data	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FShipLevel> LevelDataArray;	
};