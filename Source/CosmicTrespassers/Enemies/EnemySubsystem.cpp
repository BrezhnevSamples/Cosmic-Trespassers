// Copyright Alexander Terentyev. All Rights Reserved.

#include "EnemySubsystem.h"
#include "EnemySubsystemSettings.h"
#include "FEnemyAttackSettings.h"
#include "FEnemyStack.h"
#include "MathUtil.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Levels/LevelBound.h"
#include "Messages/MessageBlueprintFunctionLibrary.h"
#include "Utility/CollectionBlueprintFunctionLibrary.h"
#include "Utility/MathBlueprintFunctionLibrary.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	auto* Settings = GetDefault<UEnemySubsystemSettings>();

	GridData = Settings->GridData;
	
	TimeBetweenRowSpawns = Settings->TimeBetweenRowSpawns;
	EnemyClassPtrs = Settings->EnemyClassPtrs;
	MinAllowedSpawnElevation = Settings->MinAllowedSpawnElevation;
	DestroyedEnemySpeedIncreaseThresholds = Settings->DestroyedEnemySpeedIncreaseThresholds;
	InvertEnemySpeedEffect = Settings->InvertEnemySpeedEffect;

	BonusEnemyActorClassPtr = Settings->BonusEnemyActorClassPtr;
	BonusEnemySpawnInterval = Settings->BonusEnemySpawnInterval;
	BonusEnemySpawnLocation = Settings->BonusEnemySpawnLocation;

	HorizontalMoveDistance = Settings->HorizontalMoveDistance;
	VerticalMoveDistance = Settings->VerticalMoveDistance;
	MoveDelays = Settings->MoveDelays;
	MoveSounds = Settings->MoveSounds;

	AttackTimerInterval = Settings->AttackTimerInterval;
	AttackDataArray = Settings->AttackDataArray;
	
	BonusEnemyObjectPath = BonusEnemyActorClassPtr.ToSoftObjectPath();
	
	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);
	
	TotalAttackWeight = 0;

	for (FEnemyAttackSettings& AttackData : AttackDataArray)
	{		
		AttackData.RangeFrom = TotalAttackWeight;
		TotalAttackWeight += AttackData.Weight;
		AttackData.RangeTo = TotalAttackWeight;
	}
	
	Grid = FEnemyGrid(GridData);
	
	AEnemy::OnEnemyDied.AddDynamic(this, &ThisClass::OnEnemyDied);
	AEnemy::OnEnemyDied.AddDynamic(this, &ThisClass::RemoveEnemy);
	
	ALevelBound::OnLevelBoundReached.BindDynamic(this, &ThisClass::OnLevelBoundReached);	
}

void UEnemySubsystem::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
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

void UEnemySubsystem::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_SpawnEnemies))
	{
		if (UCosmicTrespassersGameInstance::Get().GetPreviousGamePhaseTag().MatchesTagExact(TAG_Game_Phase_Pause))
		{
			return;
		}
				
		DestroyBonusEnemy();

		if (EnemyHolder == nullptr)
		{
			const FTransform SpawnTransform;	
			EnemyHolder = TObjectPtr<AActor>(GetWorld()->SpawnActor(AActor::StaticClass(), &SpawnTransform));
			USceneComponent* RootComponent = Cast<USceneComponent>(EnemyHolder->AddComponentByClass(USceneComponent::StaticClass(), false, SpawnTransform, false));
			EnemyHolder->SetRootComponent(RootComponent);
		}
		
		EnemyHolder->SetActorLocation(FVector::Zero());
		ReduceSpawnElevation();
				
		StopMoving();
		SetMoveDelayIndex(FMath::Abs(CurrentSpawnElevation));

		StopAttacking();
		ClearStacks();
				
		CurrentSpawnedRowIndex = 0;

		constexpr float WaitTimeBeforeSpawn = 0.05;  
		GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::SpawnEnemies), WaitTimeBeforeSpawn, false);
				
		SpawnEnemies();	
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Gameplay))
	{
		if (UCosmicTrespassersGameInstance::Get().GetPreviousGamePhaseTag().MatchesTagExact(TAG_Game_Phase_Pause))
		{
			return;
		}			
				
		StartMoving();
		StartAttacking();
		SetBonusEnemyTimer();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		StopAttacking();
		CurrentSpawnElevation = 1;
		CurrentEnemySpeedIncreaseThresholdIndex = 0;
		StopMoving();
		ClearGrid();
		DestroyBonusEnemy();
	}
}

void UEnemySubsystem::OnGamePhaseExited(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Gameplay))
	{
		if (!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			GetWorld()->GetTimerManager().ClearTimer(BonusEnemySpawnTimerHandle);
		}		
	}
}

void UEnemySubsystem::ReduceSpawnElevation()
{
	CurrentSpawnElevation = FMath::Clamp(CurrentSpawnElevation - 1, MinAllowedSpawnElevation, 0);
}

void UEnemySubsystem::SpawnEnemies()
{	
	UWorld* World = GetWorld();
	
	for (uint8 ColumnIndex = 0; ColumnIndex < GridData.ColumnCount; ColumnIndex++)
	{		
		FEnemyGridCell Cell = Grid.Cells.Get(ColumnIndex, CurrentSpawnedRowIndex);

		FTransform SpawnTransform;

		FVector Location = Cell.GetLocation() + FVector::UpVector * CurrentSpawnElevation * GetVerticalMoveDistance();		
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(FQuat::Identity);

		constexpr uint8 RowsPerEnemyType = 2;
		
		const uint8 EnemyTypeIndex = CurrentSpawnedRowIndex / RowsPerEnemyType;		
		
		AActor* EnemyActor = World->SpawnActor(EnemyClassPtrs[EnemyTypeIndex], &SpawnTransform);		
		
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::KeepWorldTransform;		
		EnemyActor->AttachToActor(EnemyHolder, AttachmentTransformRules);
		
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		Enemy->PlaySpawnAnimation();
		Cell.SetEnemy(Enemy);
		Grid.Cells.Set(Cell, ColumnIndex, CurrentSpawnedRowIndex);

		AddEnemyToStack(ColumnIndex, Enemy);
		AddEnemy(Enemy);
	}

	if (CurrentSpawnedRowIndex < GridData.RowCount - 1)
	{
		CurrentSpawnedRowIndex++;
		GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::SpawnEnemies), TimeBetweenRowSpawns, false);
	}
	else
	{		
		GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::OnEnemiesSpawned), TimeBetweenRowSpawns, false);		
	}
}

void UEnemySubsystem::OnEnemiesSpawned()
{
	EnemySpawnTimerHandle.Invalidate();
	RemainingEnemyCount = GridData.ColumnCount * GridData.RowCount;
	
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_Gameplay);
}

void UEnemySubsystem::ClearGrid()
{
	for (uint8 x = 0; x < GridData.ColumnCount; x++)
	{
		for (uint8 y = 0; y < GridData.RowCount; y++)
		{
			FEnemyGridCell Cell = Grid.Cells.Get(x, y);

			if (!Cell.IsOccupied())
			{
				continue;
			}			

			AEnemy* Enemy = Cell.GetEnemy();			
			RemoveEnemy(Enemy);				
			Cell.SetEnemy(nullptr);			
		}
	}
}

void UEnemySubsystem::OnEnemyDied(AEnemy* Enemy)
{
	RemoveEnemyFromStack(Enemy);
	
	FGameplayTagContainer EnemyTagContainer;
	Enemy->GetOwnedGameplayTags(EnemyTagContainer);

	if (EnemyTagContainer.HasTagExact(TAG_Enemy_Bonus))
	{
		DestroyBonusEnemy();
		return;
	}
	
	RemainingEnemyCount--;

	if (CurrentEnemySpeedIncreaseThresholdIndex < DestroyedEnemySpeedIncreaseThresholds.Num())
	{
		const float DestroyedEnemyPercentage = FMathf::Clamp(1.0 - static_cast<float>(RemainingEnemyCount) / GridData.GetTotalNumberOfCells(), 0.0, 1.0);		
		
		if (DestroyedEnemyPercentage >= DestroyedEnemySpeedIncreaseThresholds[CurrentEnemySpeedIncreaseThresholdIndex])
		{
			IncreaseMoveDelayIndex();
			CurrentEnemySpeedIncreaseThresholdIndex++;
		}
	}	
	
	if (RemainingEnemyCount <= 0)
	{
		UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered,TAG_Game_Phase_SpawnEnemies);
	}
}

void UEnemySubsystem::SetBonusEnemyTimer()
{
	const float SpawnTimerValue = UMathBlueprintFunctionLibrary::GetRandomFloatFromInterval(BonusEnemySpawnInterval);
	GetWorld()->GetTimerManager().SetTimer(BonusEnemySpawnTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::SpawnBonusEnemy), SpawnTimerValue, false);
}

void UEnemySubsystem::SpawnBonusEnemy()
{
	if (CurrentBonusEnemy != nullptr)
	{
		SetBonusEnemyTimer();
		return;
	}
	
	TArray<FSoftObjectPath> LoadPaths;
	LoadPaths.Emplace(BonusEnemyObjectPath);
	UCosmicTrespassersGameInstance::Get().StreamableManager.RequestAsyncLoad(LoadPaths, FStreamableDelegate::CreateUObject(this, &ThisClass::SpawnBonusEnemyDeferred));	
}

void UEnemySubsystem::SpawnBonusEnemyDeferred()
{
	FVector SpawnLocation = BonusEnemySpawnLocation;

	const bool bEnemySpawnLeft = FMath::RandBool(); 
	
	if (bEnemySpawnLeft)
	{
		SpawnLocation.Y *= -1;
	}	

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(FQuat::Identity);
	
	CurrentBonusEnemy = TObjectPtr<AEnemy>(Cast<AEnemy>(GetWorld()->SpawnActor(BonusEnemyActorClassPtr.Get(), &SpawnTransform)));

	if (!bEnemySpawnLeft)
	{
		auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(CurrentBonusEnemy->GetAbilitySystemComponent());
    	AbilitySystem->ApplyEffectToSelf(InvertEnemySpeedEffect.GetDefaultObject(), 0);	
	}	
	
	SetBonusEnemyTimer();

	UCosmicTrespassersGameInstance::Get().StreamableManager.Unload(BonusEnemyObjectPath);
}

void UEnemySubsystem::DestroyBonusEnemy()
{
	if (CurrentBonusEnemy != nullptr)
	{
		CurrentBonusEnemy->ScheduleDestroy();
		CurrentBonusEnemy = TObjectPtr<AEnemy>(nullptr);
	}
}

void UEnemySubsystem::AddEnemy(AEnemy* Enemy)
{
	Enemies.Add(TObjectPtr<AEnemy>(Enemy));
}

void UEnemySubsystem::RemoveEnemy(AEnemy* Enemy)
{
	if (Enemy != nullptr)
	{
		Enemies.Remove(Enemy);		
	}	
}

void UEnemySubsystem::StartMoving()
{
	bNeedsToMoveDown = false;
	SetMoveTimer();
}

void UEnemySubsystem::StopMoving()
{
	CurrentMoveSoundIndex = 0;
	GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
}

void UEnemySubsystem::OnLevelBoundReached()
{	
	bNeedsToMoveDown = true;
	IncreaseMoveDelayIndex();
	SetMoveTimer();
}

void UEnemySubsystem::MoveEnemies()
{
	SetMoveTimer();
	
	if (bNeedsToMoveDown)
	{		
		MoveDown(VerticalMoveDistance);
		CurrentMoveDirection *= -1;

		PlayNextMoveSound();

		bNeedsToMoveDown = false;
		
		return;
	}
	
	FVector TargetLocation = EnemyHolder->GetActorLocation();
	TargetLocation += FVector::RightVector * HorizontalMoveDistance * CurrentMoveDirection;
	EnemyHolder->SetActorLocation(TargetLocation);

	for (const TObjectPtr<AEnemy> Enemy : Enemies)
	{
		Enemy->AdvanceAnimation();
	}
	
	PlayNextMoveSound();
}

void UEnemySubsystem::MoveDown(const float MoveDistance) const
{	
	FVector TargetLocation = EnemyHolder->GetActorLocation();
	TargetLocation += FVector::DownVector * MoveDistance;
	EnemyHolder->SetActorLocation(TargetLocation);
}

void UEnemySubsystem::SetMoveTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	TimerManager.ClearTimer(MoveTimerHandle);
	TimerManager.SetTimer(MoveTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::MoveEnemies),
		MoveDelays[CurrentMoveDelayIndex], false);	
}

void UEnemySubsystem::PlayNextMoveSound()
{	
	UGameplayStatics::PlaySound2D(GetWorld(), MoveSounds[CurrentMoveSoundIndex].LoadSynchronous());
	
	CurrentMoveSoundIndex++;

	if (CurrentMoveSoundIndex < MoveSounds.Num())
	{
		return;
	}

	CurrentMoveSoundIndex = 0;
}

void UEnemySubsystem::ResetMoveDelayIndex()
{
	CurrentMoveDelayIndex = 0;
}

void UEnemySubsystem::SetMoveDelayIndex(const uint8 Index)
{
	CurrentMoveDelayIndex = FMath::Clamp(Index, 0, MoveDelays.Num() - 1);
}

void UEnemySubsystem::IncreaseMoveDelayIndex()
{
	CurrentMoveDelayIndex = FMath::Clamp(CurrentMoveDelayIndex + 1, 0, MoveDelays.Num() - 1);
}

void UEnemySubsystem::ClearStacks()
{
	EnemyStacks.Empty();
}

void UEnemySubsystem::AddEnemyToStack(const uint8 StackIndex, AEnemy* Enemy)
{
	if (EnemyStacks.Contains(StackIndex))
	{	
		EnemyStacks[StackIndex].AddEnemy(Enemy);
	}
	else
	{
		FEnemyStack NewEnemyStack = FEnemyStack();
		NewEnemyStack.AddEnemy(Enemy);
		EnemyStacks.Emplace(StackIndex, NewEnemyStack);
	}
}

void UEnemySubsystem::RemoveEnemyFromStack(AEnemy* Enemy)
{
	uint8 StackIndexToRemove = 255;
	
	for (TTuple<uint8, FEnemyStack>& EnemyStackTuple : EnemyStacks)
	{
		FEnemyStack EnemyStack = EnemyStackTuple.Value;
		
		if (!EnemyStackTuple.Value.Contains(Enemy))
		{
			continue;
		}

		bool bStackIsEmpty = false;
		EnemyStack.RemoveEnemy(Enemy, bStackIsEmpty);		
		
		if (!bStackIsEmpty)
		{
			EnemyStackTuple.Value = EnemyStack;
		}
		else
		{
			StackIndexToRemove = EnemyStackTuple.Key;	
		}
		
		break;
	}

	if (StackIndexToRemove == 255)
	{
		return;
	}

	EnemyStacks.Remove(StackIndexToRemove);
}

void UEnemySubsystem::StartAttacking()
{
	ScheduleAttack();
}

void UEnemySubsystem::StopAttacking()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

FEnemyStack UEnemySubsystem::GetRandomEnemyStack()
{	
	return UCollectionBlueprintFunctionLibrary::GetRandomMapTuple(&EnemyStacks).Value;
}

void UEnemySubsystem::ScheduleAttack()
{
	const float AttackTimer = UMathBlueprintFunctionLibrary::GetRandomFloatFromInterval(AttackTimerInterval);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::PerformAttack), AttackTimer, false);
}

void UEnemySubsystem::PerformAttack()
{	
	const FEnemyAttackSettings AttackData = GetRandomAttackData();
	
	uint8 RemainingNumberOfAttacks = FMath::Clamp(AttackData.NumberOfAttacks, 1, EnemyStacks.Num());	

	TMap<uint8, FEnemyStack> EnemyStacksCopy = TMap(EnemyStacks);

	while (RemainingNumberOfAttacks > 0)
	{
		TTuple<uint8, FEnemyStack> EnemyStackTuple = UCollectionBlueprintFunctionLibrary::GetRandomMapTuple(&EnemyStacksCopy);

		const AEnemy* Enemy = EnemyStackTuple.Value.GetFirstEnemy();
		EnemyStacksCopy.Remove(EnemyStackTuple.Key);

		if (Enemy == nullptr)
		{
			RemainingNumberOfAttacks--;
			continue;
		}
		
		auto* EnemyAbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(Enemy->GetAbilitySystemComponent());
		EnemyAbilitySystem->TryActivateAbilityByTag(TAG_Enemy_Ability_Attack);
		
		RemainingNumberOfAttacks--;
	}
	
	ScheduleAttack();
}

FEnemyAttackSettings UEnemySubsystem::GetRandomAttackData()
{
	checkf(AttackDataArray.Num() > 0, TEXT("No attack data assigned to EnemyAttackController!"));
	
	const float RandomValue = FMath::RandRange(0.0, 1.0) * TotalAttackWeight;	
	
	FEnemyAttackSettings RandomAttackData = AttackDataArray[0];
	
	for (const FEnemyAttackSettings AttackData : AttackDataArray)
	{
		if (AttackData.RangeFrom < RandomValue || RandomValue > AttackData.RangeTo)
		{
			continue;
		}
		
		RandomAttackData = AttackData;
		break;
	}

	return RandomAttackData;
}