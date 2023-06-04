// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Common/DestroyableInterface.h"
#include "Messages/FGamePhaseMessage.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class COSMICTRESPASSERS_API AProjectile : public AActor, public IDestroyableInterface, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	AProjectile();
	virtual void Tick(float DeltaSeconds) override;

	virtual void ScheduleDestroy() override;	

protected:
	UFUNCTION()	
	virtual void BeginPlay() override;
	
	//~=============================================================================
	// Tags	
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	void SetOwnerTag(const FGameplayTag OwnerTag);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer ProjectileTagContainer;
	
	//~=============================================================================
	// Game State
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& GamePhaseMessage);	
	void OnGamePhaseEntered(const FGameplayTag GameStateTag);	
	
	//~=============================================================================
	// Visuals
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSprite;
	
	//~=============================================================================
	// Movement	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	//~=============================================================================
	// Collisions
private:
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	//~=============================================================================
	// Abilities
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UProjectileAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> DamageEffect;

	//~=============================================================================
	// Effects
public:
	void SetPlayExplosionEffect(const bool Value) { bPlayExplosionEffect = Value; }
	bool GetPlayExplosionEffect() const { return bPlayExplosionEffect; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> TrailParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TrailParticleSpawnRateName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TrailParticleSpawnRate = 15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MaxTrailParticleYOffsetName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxTrailParticleYOffset = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MinTrailParticleColorName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MaxTrailParticleColorName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor MinTrailParticleColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor MaxTrailParticleColor;

private:
	bool bPlayExplosionEffect = false;
};