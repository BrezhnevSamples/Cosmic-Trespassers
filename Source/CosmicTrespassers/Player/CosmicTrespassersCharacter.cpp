// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersCharacter.h"
#include "AbilitySystemComponent.h"
#include "CosmicTrespassersPlayerController.h"
#include "PaperSpriteComponent.h"
#include "ShipUpgrader.h"
#include "Common/CommonAttributeSet.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Weapons/WeaponComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

ACosmicTrespassersCharacter::ACosmicTrespassersCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(TG_PostPhysics);

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");	

	const FVector InitialSpriteRotation = FVector(0, 0, 90);	
	PaperSprite->SetWorldRotation(FRotator::MakeFromEuler(InitialSpriteRotation));
	
	SetRootComponent(PaperSprite);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");
	AbilitySystem = CreateDefaultSubobject<UCosmicTrespassersAbilitySystemComponent>("AbilitySystem");
	Weapon = CreateDefaultSubobject<UWeaponComponent>("Weapon");
	ShipUpgrader = CreateDefaultSubobject<UShipUpgrader>("ShipUpgrader");	
}

void ACosmicTrespassersCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystem.Get()->InitAbilityActorInfo(this, this);
}

void ACosmicTrespassersCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCosmicTrespassersGameInstance& GameInstance = UCosmicTrespassersGameInstance::Get();
	
	GameInstance.SetCharacter(this);
	GameInstance.GetSubsystem<UGameplayMessageSubsystem>()->RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);

	StartLocation = GetActorLocation();
	
	AbilitySystem->Init();
	ShipUpgrader->Init();
}

void ACosmicTrespassersCharacter::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
}

void ACosmicTrespassersCharacter::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_SpawnEnemies))
	{
		SetActorLocation(StartLocation, false, nullptr, ETeleportType::ResetPhysics);
				
		if (!AbilitySystem->HealthIsMax())
		{
			AbilitySystem.Get()->ApplyEffectToSelf(Heal1HealthEffect, 1);
		}	
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		AbilitySystem.Get()->ApplyInitEffect();
	}
}

void ACosmicTrespassersCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(CharacterTagContainer);
}

UAbilitySystemComponent* ACosmicTrespassersCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}

UWeaponComponent* ACosmicTrespassersCharacter::GetWeaponComponent() const
{
	return Weapon.Get();
}

void ACosmicTrespassersCharacter::SetSprite(UPaperSprite* NewSprite) const
{
	PaperSprite->SetSprite(NewSprite);
}

FTransform ACosmicTrespassersCharacter::GetProjectileOriginTransform() const
{
	const FName ProjectileOriginSocketName = TEXT("ProjectileOrigin");
	FTransform ProjectileOriginSocketTransform = PaperSprite->GetSocketTransform(ProjectileOriginSocketName);

	ProjectileOriginSocketTransform.SetScale3D(FVector::One());
	
	return ProjectileOriginSocketTransform;
}

void ACosmicTrespassersCharacter::MoveHorizontally(FVector InputVector) const
{
	auto* CommonAttributeSet = Cast<UCommonAttributeSet>(AbilitySystem.Get()->GetAttributeSet(UCommonAttributeSet::StaticClass()));
	InputVector.Y *= CommonAttributeSet->GetMoveSpeed() * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());	
	
	PawnMovement->AddInputVector(InputVector);
}

void ACosmicTrespassersCharacter::SetProjectileClass(const TSubclassOf<AProjectile> ProjectileClass) const
{
	GetWeaponComponent()->SetProjectileClass(ProjectileClass);
}

void ACosmicTrespassersCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	const float NewHealth = ChangeData.NewValue;
	
	if (NewHealth < ChangeData.OldValue)
	{
		ShipUpgrader->ResetUpgradeProgress();
	}
	
	HealthChanged.Broadcast(NewHealth);	
}