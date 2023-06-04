// Copyright Alexander Terentyev. All Rights Reserved.

#include "ProjectileAbilitySystemComponent.h"
#include "Common/CommonAttributeSet.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UProjectileAbilitySystemComponent::Init()
{
	Super::Init();
	
	auto* CommonAttributeSet = Cast<UCommonAttributeSet>(GetAttributeSet(UCommonAttributeSet::StaticClass()));
	auto* MovementComponent = Cast<UProjectileMovementComponent>(GetOwner()->FindComponentByClass(UProjectileMovementComponent::StaticClass()));

	const float MoveSpeed = CommonAttributeSet->GetMoveSpeed();
	MovementComponent->MaxSpeed = MoveSpeed;
	
	FVector Velocity = MovementComponent->Velocity;	
	Velocity = Velocity.GetSafeNormal() * MoveSpeed;
	MovementComponent->Velocity = Velocity;
	MovementComponent->UpdateComponentVelocity();	
}