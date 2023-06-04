// Copyright Alexander Terentyev. All Rights Reserved.

#include "Projectile.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "PaperSpriteComponent.h"
#include "ProjectileAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Framework/CosmicTrespassersPlayerState.h"
#include "Messages/FGamePhaseMessage.h"
#include "Tags/CosmicTrespassersNativeTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

class ACosmicTrespassersPlayerState;

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;	

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
	SetRootComponent(PaperSprite);	
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->ProjectileGravityScale = 0;

	AbilitySystem = CreateDefaultSubobject<UProjectileAbilitySystemComponent>("AbilitySystem");

	TrailParticleSystem = CreateDefaultSubobject<UNiagaraComponent>("TrailParticles");
	TrailParticleSystem->SetupAttachment(PaperSprite, TEXT("TrailOrigin"));
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bDestroyScheduled)
	{
		Destroy();
	}
}

void AProjectile::ScheduleDestroy()
{
	IDestroyableInterface::ScheduleDestroy();
	SetActorEnableCollision(false);
}

void AProjectile::BeginPlay()
{	
	Super::BeginPlay();
	
	OnActorHit.AddDynamic(this, &AProjectile::OnHit);
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);
	
	AbilitySystem->InitAbilityActorInfo(this, this);
	AbilitySystem->Init();

	TrailParticleSystem->SetFloatParameter(TrailParticleSpawnRateName, TrailParticleSpawnRate);
	TrailParticleSystem->SetFloatParameter(MaxTrailParticleYOffsetName, MaxTrailParticleYOffset);
	TrailParticleSystem->SetColorParameter(MinTrailParticleColorName, MinTrailParticleColor);
	TrailParticleSystem->SetColorParameter(MaxTrailParticleColorName, MaxTrailParticleColor);	
}

void AProjectile::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ProjectileTagContainer);
}

void AProjectile::SetOwnerTag(const FGameplayTag OwnerTag)
{
	ProjectileTagContainer.AddTag(OwnerTag);
}

void AProjectile::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& GamePhaseMessage)
{
	if (GamePhaseMessage.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(GamePhaseMessage.GamePhaseTag);
	}
}

void AProjectile::OnGamePhaseEntered(const FGameplayTag GameStateTag)
{
	if (GameStateTag.MatchesTagExact(TAG_Game_Phase_SpawnEnemies) || GameStateTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		if (HasMatchingGameplayTag(TAG_Ship))
		{
			UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->DecreaseTotalShotCounter();	
		}

		ScheduleDestroy();
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void AProjectile::OnHit(AActor*, AActor* OtherActor, FVector, const FHitResult&)
{
	if (OtherActor != Owner)
    {	
		FGameplayEventData EventData;
		EventData.OptionalObject = OtherActor;
	
		AbilitySystem->HandleGameplayEvent(TAG_Projectile_Ability_Hit_Trigger, &EventData);     	
    }	
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AProjectile::OnBeginOverlap(AActor*, AActor* OtherActor)
{
	if (OtherActor == Owner)
	{	
		return;
	}

	// Enemies can't shoot other enemies. They're nice like that.
	if (const UCosmicTrespassersGameInstance& GameInstance = UCosmicTrespassersGameInstance::Get();	!ProjectileTagContainer.HasTagExact(TAG_Enemy) || !GameInstance.IsEnemy(OtherActor))
	{
		FGameplayEventData EventData;
		EventData.OptionalObject = OtherActor;

		AbilitySystem->HandleGameplayEvent(TAG_Projectile_Ability_Hit_Trigger, &EventData);
	}
}

UAbilitySystemComponent* AProjectile::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}