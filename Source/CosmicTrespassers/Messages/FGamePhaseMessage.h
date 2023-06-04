// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "FGamePhaseMessage.generated.h"

USTRUCT(BlueprintType)
struct COSMICTRESPASSERS_API FGamePhaseMessage
{
	GENERATED_BODY()
	
	//~=============================================================================
	// Data
public:
	UPROPERTY(BlueprintReadWrite, Category=Notification)
	FGameplayTag GamePhaseEventTag; // Holds "Game phase entered" or "Game phase exited" tag

	UPROPERTY(BlueprintReadWrite, Category=Notification)
	FGameplayTag GamePhaseTag;
};