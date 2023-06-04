// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "PaperSprite.h"
#include "Common/HealthInterface.h"
#include "Messages/FGamePhaseMessage.h"
#include "Weapons/WeaponUserInterface.h"
#include "GameFramework/Pawn.h"
#include "CosmicTrespassersCharacter.generated.h"

class AProjectile;

DECLARE_EVENT_OneParam(ACosmicTrespassersCharacter, FOnHealthChanged, float)

UCLASS()
class COSMICTRESPASSERS_API ACosmicTrespassersCharacter : public APawn, public IWeaponUserInterface, public IAbilitySystemInterface, public IHealthInterface,
											   public IGameplayTagAssetInterface
											   
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:	
	ACosmicTrespassersCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

private:
	FVector StartLocation;
	
	//~=============================================================================
	// Game phase
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	
	//~=============================================================================
	// Tags
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CharacterTagContainer;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UShipUpgrader> ShipUpgrader;

	//~=============================================================================
	// Visuals
public:
	void SetSprite(UPaperSprite* NewSprite) const;
	virtual FTransform GetProjectileOriginTransform() const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSprite;
	
	//~=============================================================================
	// Movement
public:
	void MoveHorizontally(FVector InputVector) const;

	//~=============================================================================
	// Weapon
public:
	void SetProjectileClass(const TSubclassOf<AProjectile> ProjectileClass) const;

	//~=============================================================================
	// Health
public:
	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData) override;
	FOnHealthChanged& OnHealthChanged() { return HealthChanged; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> Heal1HealthEffect;
	
private:
	FOnHealthChanged HealthChanged;
};