// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SaveSubsystem.generated.h"

const FString SaveSlotName = "SaveSlot";
constexpr int32 UserIndex = 0;

UCLASS()
class COSMICTRESPASSERS_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	//~=============================================================================
	// Save Game
public:
	void SaveHiScore(const uint32 HiScore) const;
	uint32 GetHiScore() const;

private:
	UPROPERTY()
	TObjectPtr<class UCosmicTrespassersSaveGame> SaveGame;
};