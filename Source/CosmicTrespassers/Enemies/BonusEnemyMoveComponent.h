// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "BonusEnemyMoveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRESPASSERS_API UBonusEnemyMoveComponent : public UActorComponent
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance		
public:	
	UBonusEnemyMoveComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

	//~=============================================================================
	// Abilities	
private:	
	TObjectPtr<class UAbilitySystemComponent> AbilitySystem;
};
