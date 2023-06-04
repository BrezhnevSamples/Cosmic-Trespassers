// Copyright Alexander Terentyev. All Rights Reserved.

#include "ExplosionEffect.h"
#include "PaperFlipbookComponent.h"

AExplosionEffect::AExplosionEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");	

	const FVector InitialRotation = FVector(0, 0, 90);	
	Flipbook->SetWorldRotation(FRotator::MakeFromEuler(InitialRotation));

	SetRootComponent(Flipbook);
}

void AExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
	const float AnimationDuration = Flipbook->GetFlipbookLength();
	SetLifeSpan(AnimationDuration);
}