// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ActorComponent.h"
#include "Weapons/Projectile.h"
#include "BunkerPart.generated.h"

class ABunker;

UCLASS(BlueprintType)
class COSMICTRESPASSERS_API ABunkerPart : public AActor, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	ABunkerPart();

	void Enable();
	void Disable();
	
	void SetBunker(ABunker* OwnerBunker) { Bunker = OwnerBunker; }
private:
	UPROPERTY()
	TObjectPtr<ABunker> Bunker;
	
protected:
	virtual void BeginPlay() override;
	
	//~=============================================================================
	// Tags
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer BunkerPartTagContainer;
	
	//~=============================================================================
	// Components
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCosmicTrespassersAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSprite;
	
	//~=============================================================================
	// Damage
public:
	void OnDamageTaken(const AProjectile* Projectile);
};