// Copyright Alexander Terentyev. All Rights Reserved.

#include "BunkerPart.h"
#include "Bunker.h"
#include "PaperSpriteComponent.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Weapons/Projectile.h"

ABunkerPart::ABunkerPart()
{	
	PrimaryActorTick.bCanEverTick = false;

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");

	const FVector InitialSpriteRotation = FVector(0, 0, 90);	
	PaperSprite->SetWorldRotation(FRotator::MakeFromEuler(InitialSpriteRotation));

	SetRootComponent(PaperSprite);

	AbilitySystem = CreateDefaultSubobject<UCosmicTrespassersAbilitySystemComponent>("AbilitySystem");
}

void ABunkerPart::Enable()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ABunkerPart::Disable()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ABunkerPart::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystem->Init();
}

void ABunkerPart::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(BunkerPartTagContainer);
}

UAbilitySystemComponent* ABunkerPart::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}

void ABunkerPart::OnDamageTaken(const AProjectile* Projectile)
{
	Bunker->OnBunkerPartHit(this, Projectile);
	Disable();
}