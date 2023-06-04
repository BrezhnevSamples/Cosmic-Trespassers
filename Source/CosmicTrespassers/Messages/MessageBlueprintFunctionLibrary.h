// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MessageBlueprintFunctionLibrary.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UMessageBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	//~=============================================================================
    	// Game phase
    public:
    	UFUNCTION(BlueprintCallable, Category = "Cosmis Trespassers Messages")
    	static void BroadcastGamePhaseMessage(const FGameplayTag GamePhaseEventTag, const FGameplayTag GamePhaseTag);
};
