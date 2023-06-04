// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Messages/FGamePhaseMessage.h"
#include "Tags/CosmicTrespassersNativeTags.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "CosmicTrespassersGameInstance.generated.h"

class ACosmicTrespassersGameState;

DECLARE_EVENT(UCosmicTrespassersGameInstance, FOnInitialized)

UCLASS()
class COSMICTRESPASSERS_API UCosmicTrespassersGameInstance : public UGameInstance
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	static UCosmicTrespassersGameInstance& Get() { return *Instance; }

	UFUNCTION(BlueprintCallable)
	static UCosmicTrespassersGameInstance* GetGameInstance() { return Instance.Get(); }
	
	virtual void Init() override;	

private:
	static TObjectPtr<UCosmicTrespassersGameInstance> Instance;

	void InitIfReady();

	//~=============================================================================
	// Game State
public:
	static void StartGame();

private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);

	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	void OnGamePhaseExited(const FGameplayTag GamePhaseTag);	
	
	//~=============================================================================
	// Managers
public:	
	FStreamableManager StreamableManager;

	//~=============================================================================
	// Character	
public:
	template<class T>
	T* GetCharacter() {	return Cast<T>(Character);	}

	template<class T>
	T* GetPlayerController() { return Cast<T>(PlayerController); }

	template<class T>
	T* GetPlayerState() { return Cast<T>(UGameplayStatics::GetPlayerState(GetWorld(), 0)); }
	
	void SetCharacter(APawn* CosmicTrespassersCharacter);
	void SetPlayerController(APlayerController* CosmicTrespassersPlayerController);

	bool IsPlayerCharacter(AActor* Actor) const;
	bool IsEnemy(AActor* Actor) const;
	
private:
	UPROPERTY()
	TObjectPtr<APawn> Character;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	//~=============================================================================
	// Game phase
public:
	void TogglePause();

	FGameplayTag GetPreviousGamePhaseTag() const { return PreviousGamePhaseTag; }
private:	
	bool bPaused;
	
	FGameplayTag CurrentGamePhaseTag = TAG_Game_Phase_Splash;
	FGameplayTag PreviousGamePhaseTag = TAG_Game_Phase_Splash;
	FGameplayTag GamePhaseBeforePauseTag;	
};