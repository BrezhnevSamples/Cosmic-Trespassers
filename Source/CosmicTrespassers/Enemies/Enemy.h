// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Common/DestroyableInterface.h"
#include "Messages/FGamePhaseMessage.h"
#include "Weapons/WeaponUserInterface.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBonusEnemyDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AEnemy*, Enemy);

UCLASS()
class COSMICTRESPASSERS_API AEnemy : public APawn, public IWeaponUserInterface, public IAbilitySystemInterface, public IDestroyableInterface,
						   public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	virtual void ScheduleDestroy() override;

protected:
	virtual void BeginPlay() override;	

	//~=============================================================================
	// Game State
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& GamePhaseMessage);	
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag) const;
	
	//~=============================================================================
	// Tags
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual FTransform GetProjectileOriginTransform() const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer EnemyTagContainer;
	
	//~=============================================================================
	// Components
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual UWeaponComponent* GetWeaponComponent() const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UFloatingPawnMovement> PawnMovement;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCosmicTrespassersAbilitySystemComponent> AbilitySystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWeaponComponent> Weapon;

	//~=============================================================================
	// Visuals
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySpawnAnimation();
	
	void AdvanceAnimation();
	
protected:		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbook> Flipbook;
	
private:	
	uint8 CurrentSpriteIndex;

	//~=============================================================================
	// Sounds
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundWave> BonusEnemySound;
	
private:
	TObjectPtr<UAudioComponent> BonusEnemyAudioComponent;

	//~=============================================================================
	// Collisions
private:
	UFUNCTION()
	void OnBeginOverlap(AActor* Self, AActor* OtherActor);
	
	//~=============================================================================
	// Events
public:	
	static FOnEnemyDied OnEnemyDied;	

	//~=============================================================================
	// Score
public:
	uint16 GetDestroyScore() const;
};