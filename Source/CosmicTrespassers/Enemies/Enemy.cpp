// Copyright Alexander Terentyev. All Rights Reserved.

#include "Enemy.h"
#include "EnemyAttributeSet.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Messages/FGamePhaseMessage.h"
#include "Tags/CosmicTrespassersNativeTags.h"
#include "Weapons/WeaponComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

FOnEnemyDied AEnemy::OnEnemyDied;

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");

	const FVector InitialSpriteRotation = FVector(0, 0, 90);	
	PaperSprite->SetWorldRotation(FRotator::MakeFromEuler(InitialSpriteRotation));	
	
	SetRootComponent(PaperSprite);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	AbilitySystem = CreateDefaultSubobject<UCosmicTrespassersAbilitySystemComponent>("AbilitySystem");
	Weapon = CreateDefaultSubobject<UWeaponComponent>("Weapon");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystem.Get()->InitAbilityActorInfo(this, this);
	AbilitySystem->Init();
	OnActorBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);	

	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);

	if (EnemyTagContainer.HasTagExact(TAG_Enemy_Bonus))
	{
		BonusEnemyAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BonusEnemySound);		
	}	
}

void AEnemy::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& GamePhaseMessage)
{
	if (GamePhaseMessage.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(GamePhaseMessage.GamePhaseTag);
	}
	else
	{
		OnGamePhaseExited(GamePhaseMessage.GamePhaseTag);
	}
}

void AEnemy::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		ScheduleDestroy();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Pause))
	{
		if (EnemyTagContainer.HasTagExact(TAG_Enemy_Bonus) && BonusEnemyAudioComponent != nullptr)
		{
			BonusEnemyAudioComponent->SetPaused(true);
		}		
	}
}

void AEnemy::OnGamePhaseExited(const FGameplayTag GamePhaseTag) const
{	
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Pause))
	{
		if (EnemyTagContainer.HasTagExact(TAG_Enemy_Bonus) && BonusEnemyAudioComponent != nullptr)
		{
			BonusEnemyAudioComponent->SetPaused(false);	
		}		
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDestroyScheduled)
	{
		Destroy();

		if (EnemyTagContainer.HasTagExact(TAG_Enemy_Bonus))
		{
			BonusEnemyAudioComponent->Deactivate();
			BonusEnemyAudioComponent->DestroyComponent();
		}
	}
}

void AEnemy::ScheduleDestroy()
{
	if (!bDestroyScheduled)
	{
		IDestroyableInterface::ScheduleDestroy();
		SetActorEnableCollision(false);
	}
}

void AEnemy::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(EnemyTagContainer);
}

FTransform AEnemy::GetProjectileOriginTransform() const
{
	const FName ProjectileOriginSocketName = TEXT("ProjectileOrigin");
	FTransform ProjectileOriginSocketTransform = PaperSprite->GetSocketTransform(ProjectileOriginSocketName);

	ProjectileOriginSocketTransform.SetScale3D(FVector::One());
	
	return ProjectileOriginSocketTransform;
}

UAbilitySystemComponent* AEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}

UWeaponComponent* AEnemy::GetWeaponComponent() const
{
	return Weapon.Get();
}

void AEnemy::AdvanceAnimation()
{
	if (Flipbook == nullptr)
	{
		return;
	}

	CurrentSpriteIndex++;

	if (constexpr uint8 MaxSpriteIndex = 2; CurrentSpriteIndex >= MaxSpriteIndex)
	{
		CurrentSpriteIndex = 0;
	}

	PaperSprite->SetSprite(Flipbook.Get()->GetSpriteAtFrame(CurrentSpriteIndex));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AEnemy::OnBeginOverlap(AActor*, AActor* OtherActor)
{
	const auto* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(OtherActor);

	if (GameplayTagAssetInterface != nullptr && GameplayTagAssetInterface->HasMatchingGameplayTag(TAG_Bunker))
	{
		const FGameplayEventData GameplayEventData;		
		Cast<IAbilitySystemInterface>(OtherActor)->GetAbilitySystemComponent()->HandleGameplayEvent(TAG_Bunker_Ability_DestroyPart_Trigger, &GameplayEventData);
	}
}

uint16 AEnemy::GetDestroyScore() const
{
	const auto* EnemyAttributeSet = Cast<UEnemyAttributeSet>(AbilitySystem.Get()->GetAttributeSet(UEnemyAttributeSet::StaticClass()));
	return EnemyAttributeSet->GetDestroyScore();
}