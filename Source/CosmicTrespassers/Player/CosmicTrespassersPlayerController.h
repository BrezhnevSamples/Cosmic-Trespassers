// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Messages/FGamePhaseMessage.h"
#include "GameFramework/PlayerController.h"
#include "CosmicTrespassersPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinuePressed);

UCLASS()
class COSMICTRESPASSERS_API ACosmicTrespassersPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	//~=============================================================================
	// Instance	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;	
	
private:
	TObjectPtr<class ACosmicTrespassersCharacter> CosmicTrespassersCharacter;

	//~=============================================================================
	// Game phase
private:
	void OnGamePhaseMessageReceived(FGameplayTag ChannelTag, const FGamePhaseMessage& Message);
	void OnGamePhaseEntered(const FGameplayTag GamePhaseTag);
	
	//~=============================================================================
	// Input
public:	
	FOnContinuePressed OnContinuePressed;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputActionConfig> InputActionConfig;

private:	
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	
	void AddStartMenuInputBindings();
	void AddGameplayInputBindings();
	void AddGameOverInputBindings();
	void AddPauseInputBinding();

	void ClearInputBindings() const;

	UFUNCTION()
	// ReSharper disable once CppMemberFunctionMayBeConst
	void OnContinueKeyPressed() { OnContinuePressed.Broadcast(); }
	void AddHorizontalInput(const FInputActionValue& InputActionValue);
	void OnFireButtonPressed();
	void OnTogglePauseButtonPressed();

	//~=============================================================================
	// Character
private:
	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	
	//~=============================================================================
	// Camera	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> DamageCameraShake;

private:
	void PlayDamageCameraShake();
};