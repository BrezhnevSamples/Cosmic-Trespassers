// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosionEffect.generated.h"

UCLASS()
class COSMICTRESPASSERS_API AExplosionEffect : public AActor
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance	
public:	
	AExplosionEffect();
	
protected:
	virtual void BeginPlay() override;

	//~=============================================================================
	// Flipbook
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> Flipbook;
};