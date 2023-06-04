// Copyright Alexander Terentyev. All Rights Reserved.

#include "Bunker.h"
#include "BunkerPart.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"

ABunker::ABunker()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABunker::BeginPlay()
{
	Super::BeginPlay();	
	SpawnBunkerParts();	

	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);

	TotalPatternWeight = 0;

	for (FBunkerDamagePattern& DamagePattern : DamagePatterns)
	{
		DamagePattern.RangeFrom = TotalPatternWeight;
		TotalPatternWeight += DamagePattern.Weight;
		DamagePattern.RangeTo = TotalPatternWeight;
	}
}

void ABunker::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
	else if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Exited))
	{
		OnGamePhaseExited(Message.GamePhaseTag);
	}
}

void ABunker::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		DisableBunkerParts();
	}
}

void ABunker::OnGamePhaseExited(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		EnableBunkerParts();
	}
}

void ABunker::SpawnBunkerParts()
{
	UWorld* World = GetWorld();

	constexpr uint16 MaxBunkerWidth = 26;
	
	const float StartY = GetActorLocation().Y - (MaxBunkerWidth - 1) * DistanceBetweenParts * 0.5;

	const uint8 NumberOfBunkerRows = SpawnDataArray.Num(); 
	const float StartZ = GetActorLocation().Z + (NumberOfBunkerRows - 1) * DistanceBetweenParts * 0.5;

	for (uint8 RowIndex = 0; RowIndex < NumberOfBunkerRows; RowIndex++)
	{
		for (const FInt32Interval PartColumnInterval : SpawnDataArray[RowIndex].PartColumnIntervals)
		{
			for (int32 ColumnIndex = PartColumnInterval.Min; ColumnIndex < PartColumnInterval.Max; ColumnIndex++)
			{
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(FVector(0, StartY + ColumnIndex * DistanceBetweenParts, StartZ - RowIndex * DistanceBetweenParts));
				SpawnTransform.SetRotation(FQuat::Identity);
				
				AActor* BunkerPartActor = World->SpawnActor(BunkerPartBP, &SpawnTransform);
				BunkerPartActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

				ABunkerPart* BunkerPart = Cast<ABunkerPart>(BunkerPartActor);
				BunkerPart->SetBunker(this);
				
				BunkerParts.Emplace(BunkerPart);
			}
		}
	}
}

void ABunker::DisableBunkerParts()
{
	for (const TObjectPtr<ABunkerPart>& BunkerPart : BunkerParts)
	{
		BunkerPart->Disable();
	}
}

void ABunker::EnableBunkerParts()
{
	for (const TObjectPtr<ABunkerPart>& BunkerPart : BunkerParts)
	{
		BunkerPart->Enable();
	}
}

void ABunker::OnBunkerPartHit(const AActor* BunkerPart, const AActor* Projectile) const
{
	AActor* ProjectileOwner = Projectile->GetOwner();
    
    bool ApplyPatternUpwards = false;
    
    if (UCosmicTrespassersGameInstance::Get().IsPlayerCharacter(ProjectileOwner))
    {
    	ApplyPatternUpwards = true;
    }	
    
    FBunkerDamagePattern DamagePattern = SelectRandomDamagePattern();

    UWorld* World = GetWorld();
    FVector HitLocation = BunkerPart->GetActorLocation();
    
    for (FVector DamageOffset : DamagePattern.DamageOffsets)
    {
    	if (!ApplyPatternUpwards)
    	{
    		DamageOffset.Z *= -1;
    	}

    	constexpr float TraceOffsetX = 5;
    	constexpr float TraceLength = 10;
    	
    	FVector TraceStartLocation = HitLocation + FVector(-TraceOffsetX, DamageOffset.Y * DistanceBetweenParts, DamageOffset.Z * DistanceBetweenParts);
    	FVector TraceEndLocation = TraceStartLocation + FVector::ForwardVector * TraceLength;
    	
    	FHitResult HitResult;
    	
    	FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(TargetObjectType);
    	
    	if (!World->LineTraceSingleByObjectType(HitResult, TraceStartLocation, TraceEndLocation, ObjectQueryParams))
    	{
    		UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FLinearColor::Red, 100, 1);
    		continue;
    	}
    	
    	auto* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitResult.GetActor());

    	if (AbilitySystemInterface == nullptr)
    	{
    		return;
    	}

    	auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());		
    	AbilitySystem->ApplyEffectToSelf(DamageEffect, 1);
    }
}

FBunkerDamagePattern ABunker::SelectRandomDamagePattern() const
{
	const float RandomValue = FMath::RandRange(0.0, 1.0) * TotalPatternWeight;
    
    for (FBunkerDamagePattern DamagePattern : DamagePatterns)
    {
    	if (RandomValue < DamagePattern.RangeFrom || RandomValue > DamagePattern.RangeTo)
    	{
    		continue;
    	}

    	return DamagePattern;
    }
    	
    return DamagePatterns[0];
}