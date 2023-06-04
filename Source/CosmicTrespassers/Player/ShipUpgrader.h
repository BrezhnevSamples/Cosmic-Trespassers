// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "FShipLevel.h"
#include "Components/ActorComponent.h"
#include "Messages/FGamePhaseMessage.h"
#include "ShipUpgrader.generated.h"

class UGameplayEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRESPASSERS_API UShipUpgrader : public UActorComponent
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	UShipUpgrader();
	void Init();

private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);

private:
	class UCosmicTrespassersAbilitySystemComponent* GetAbilitySystem() const;
	
	//~=============================================================================
	// Upgrade
public:
	void ResetUpgradeProgress();	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UShipProgressionConfig> ShipProgressionData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LevelUpEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ResetLevelEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> UpgradeParticleSystem;
	
private:
	uint32 AccumulatedScore;
	uint32 NextUpgradeScore;
	TArray<FSoftObjectPath> UpgradeLoadPaths;

	void ResetAccumulatedScore();
	FShipLevel GetShipLevelData(const uint8 Level) const;
	void TryToStartUpgrade(const uint32 AddedScore, const uint32 TotalScore);
	void PerformUpgrade();	
};