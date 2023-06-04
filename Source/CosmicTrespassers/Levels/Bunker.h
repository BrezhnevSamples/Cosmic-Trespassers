// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FBunkerDamagePattern.h"
#include "FBunkerRowSpawnData.h"
#include "Messages/FGamePhaseMessage.h"
#include "GameFramework/Actor.h"
#include "Bunker.generated.h"

class ABunkerPart;

constexpr float DistanceBetweenParts = 8.5;

UCLASS()
class COSMICTRESPASSERS_API ABunker : public AActor
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:	
	ABunker();

protected:
	virtual void BeginPlay() override;

	//~=============================================================================
	// Game phase
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);

	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag);
	
	//~=============================================================================
	// Parts
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABunkerPart> BunkerPartBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBunkerRowSpawnData> SpawnDataArray;
	
private:	
	TArray<TObjectPtr<ABunkerPart>> BunkerParts;

	void SpawnBunkerParts();
	void DisableBunkerParts();
	void EnableBunkerParts();
	
	//~=============================================================================
	// Damage
public:
	void OnBunkerPartHit(const AActor* BunkerPart, const AActor* Projectile) const;
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FBunkerDamagePattern> DamagePatterns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> TargetObjectType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> DamageEffect;
	
private:
	float TotalPatternWeight;
	
	FBunkerDamagePattern SelectRandomDamagePattern() const;
};