// Copyright Alexander Terentyev. All Rights Reserved.

#include "SaveSubsystem.h"
#include "CosmicTrespassersSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// We'll do saving and loading synchronously to keep things simple since the save game size is small
	SaveGame = TObjectPtr<UCosmicTrespassersSaveGame>(Cast<UCosmicTrespassersSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex)));

	if (SaveGame.Get() == nullptr)
	{
		SaveGame = TObjectPtr<UCosmicTrespassersSaveGame>(Cast<UCosmicTrespassersSaveGame>(UGameplayStatics::CreateSaveGameObject(UCosmicTrespassersSaveGame::StaticClass())));
		UGameplayStatics::SaveGameToSlot(SaveGame.Get(), SaveSlotName, UserIndex);
	}
}

void USaveSubsystem::SaveHiScore(const uint32 HiScore) const
{
	SaveGame.Get()->HiScore = HiScore;
	UGameplayStatics::SaveGameToSlot(SaveGame.Get(), SaveSlotName, UserIndex);
}

uint32 USaveSubsystem::GetHiScore() const
{
	return SaveGame.Get()->HiScore;
}