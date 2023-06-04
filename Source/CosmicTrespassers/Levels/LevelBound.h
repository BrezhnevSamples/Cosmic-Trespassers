// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ELevelBoundType.h"
#include "Messages/FGamePhaseMessage.h"
#include "LevelBound.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnLevelBoundReached);

UCLASS()
class COSMICTRESPASSERS_API ALevelBound : public AActor
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance		
public:	
	ALevelBound();
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	ELevelBoundType Type;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComponent;
	
	virtual void BeginPlay() override;

	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);

	//~=============================================================================
	// Collisions
public:
	static FOnLevelBoundReached OnLevelBoundReached;
	
private:
	bool bIgnoreCollisions = false;
	FTimerHandle CollisionEnableTimerHandle;

	UFUNCTION()
	void OnBeginOverlap(AActor* Actor, AActor* OtherActor);
	
	void EnableCollisions();
};