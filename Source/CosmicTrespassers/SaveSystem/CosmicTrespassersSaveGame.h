// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CosmicTrespassersSaveGame.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UCosmicTrespassersSaveGame : public USaveGame
{
	GENERATED_BODY()

	//~=============================================================================
	// Data
public:
	UPROPERTY()
	uint32 HiScore;
};