// Copyright Alexander Terentyev. All Rights Reserved.

#include "ShipUpgrader.h"
#include "AbilitySystemComponent.h"
#include "CosmicTrespassersCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "ShipAttributeSet.h"
#include "ShipProgressionConfig.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Score/ScoreSubsystem.h"
#include "Weapons/Projectile.h"
#include "GameFramework/GameplayMessageSubsystem.h"

UShipUpgrader::UShipUpgrader()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShipUpgrader::Init()
{
	const UCosmicTrespassersGameInstance& GameInstance = UCosmicTrespassersGameInstance::Get();	

	GameInstance.GetSubsystem<UGameplayMessageSubsystem>()->RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);	
	GameInstance.GetSubsystem<UScoreSubsystem>()->OnScoreChanged().AddUObject(this, &UShipUpgrader::TryToStartUpgrade);

	GetAbilitySystem()->GetAttributeSet(UShipAttributeSet::StaticClass());

	ResetUpgradeProgress();
}

void UShipUpgrader::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
}

void UShipUpgrader::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		ResetAccumulatedScore();
	}
}

UCosmicTrespassersAbilitySystemComponent* UShipUpgrader::GetAbilitySystem() const
{
	const auto* Character = Cast<ACosmicTrespassersCharacter>(GetOwner());	
	return Cast<UCosmicTrespassersAbilitySystemComponent>(Character->GetAbilitySystemComponent());
}

void UShipUpgrader::ResetUpgradeProgress()
{
	UCosmicTrespassersAbilitySystemComponent* AbilitySystem = GetAbilitySystem();
	
	AbilitySystem->ApplyEffectToSelf(ResetLevelEffect.GetDefaultObject(), 1);
	ResetAccumulatedScore();

	const UShipAttributeSet* ShipAttributeSet = Cast<UShipAttributeSet>(AbilitySystem->GetAttributeSet(UShipAttributeSet::StaticClass()));	
	NextUpgradeScore = GetShipLevelData(ShipAttributeSet->GetCurrentLevel() + 1).ScoreToReach;
	
	PerformUpgrade();
}

void UShipUpgrader::ResetAccumulatedScore()
{
	AccumulatedScore = 0;
}

FShipLevel UShipUpgrader::GetShipLevelData(const uint8 Level) const
{			
	return ShipProgressionData->LevelDataArray[Level - 1];
}

void UShipUpgrader::TryToStartUpgrade(const uint32 AddedScore, const uint32)
{
	AccumulatedScore += AddedScore;
	
	if (AccumulatedScore < NextUpgradeScore)
	{
		return;
	}
	
	const auto* Character = Cast<ACosmicTrespassersCharacter>(GetOwner());
	
	auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(Character->GetAbilitySystemComponent());
	AbilitySystem->ApplyEffectToSelf(LevelUpEffect.GetDefaultObject(), 0);
	
	const UShipAttributeSet* ShipAttributeSet = Cast<UShipAttributeSet>(GetAbilitySystem()->GetAttributeSet(UShipAttributeSet::StaticClass()));

	const float CurrentShipLevel = ShipAttributeSet->GetCurrentLevel();
	const float MaxShipLevel = ShipAttributeSet->GetMaxLevel();
	
	if (CurrentShipLevel == MaxShipLevel)
	{	
		UCosmicTrespassersGameInstance::Get().GetSubsystem<UScoreSubsystem>()->OnScoreChanged().RemoveAll(this);
	}
	else
	{
		NextUpgradeScore = GetShipLevelData(CurrentShipLevel + 1).ScoreToReach;
	}

	const FShipLevel CurrentShipLevelData = GetShipLevelData(CurrentShipLevel);
	
	UpgradeLoadPaths.Emplace(CurrentShipLevelData.SpriteObjectPtr.ToSoftObjectPath());
	UpgradeLoadPaths.Emplace(CurrentShipLevelData.ProjectileClassPtr.ToSoftObjectPath());	
	
	UCosmicTrespassersGameInstance::Get().StreamableManager.RequestAsyncLoad(UpgradeLoadPaths, FStreamableDelegate::CreateUObject(this, &UShipUpgrader::PerformUpgrade));
}

void UShipUpgrader::PerformUpgrade()
{
	const auto* Character = Cast<ACosmicTrespassersCharacter>(GetOwner());	

	auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(Character->GetAbilitySystemComponent());

	const UShipAttributeSet* ShipAttributeSet = Cast<UShipAttributeSet>(AbilitySystem->GetAttributeSet(UShipAttributeSet::StaticClass()));

	const float CurrentLevel = ShipAttributeSet->GetCurrentLevel();
	
	const FShipLevel CurrentShipLevelData = GetShipLevelData(CurrentLevel);
	
	for (FShipAttribute ShipAttributeData : CurrentShipLevelData.ShipAttributeDataArray)
	{
		AbilitySystem->SetNumericAttributeBase(ShipAttributeData.GameplayAttribute, ShipAttributeData.Value);	
	}
	
	UPaperSprite* NewSprite = CurrentShipLevelData.SpriteObjectPtr.Get();
	const TSubclassOf<AProjectile> ProjectileClass = CurrentShipLevelData.ProjectileClassPtr.Get();

	Character->SetSprite(NewSprite);
	Character->SetProjectileClass(ProjectileClass);

	if (CurrentLevel != 1)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, UpgradeParticleSystem, Character->GetActorLocation());	
	}	
	
	UpgradeLoadPaths.Empty();
}