// Copyright Alexander Terentyev. All Rights Reserved.

#include "BonusEnemyMoveComponent.h"
#include "Enemy.h"
#include "Common/CommonAttributeSet.h"

UBonusEnemyMoveComponent::UBonusEnemyMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBonusEnemyMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystem = Cast<AEnemy>(GetOwner())->GetAbilitySystemComponent();
}

void UBonusEnemyMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* CommonAttributeSet = Cast<UCommonAttributeSet>(AbilitySystem->GetAttributeSet(UCommonAttributeSet::StaticClass()));
	const float MoveSpeed = CommonAttributeSet->GetMoveSpeed();

	AActor* Owner = GetOwner();
	
	FVector TargetLocation = Owner->GetActorLocation();
	TargetLocation += FVector::RightVector * MoveSpeed * DeltaTime;
	Owner->SetActorLocation(TargetLocation);

	if (constexpr float KillY = 1350; FMath::Abs(Owner->GetActorLocation().Y) >= KillY)
	{
		Cast<AEnemy>(Owner)->ScheduleDestroy();
	}
}
