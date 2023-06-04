// Copyright Underdork Studios. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "Engine/DataAsset.h"
#include "InputActionConfig.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UInputActionConfig : public UDataAsset
{
	GENERATED_BODY()

	//~=============================================================================
	// Mappings
public:
	UInputMappingContext* GetInputMappingContext() const { return InputMappingContext.Get(); }
	UInputAction* GetMoveHorizontallyAction() const { return MoveHorizontallyAction.Get(); }
	UInputAction* GetFireAction() const { return FireAction.Get(); }
	UInputAction* GetPauseAction() const { return PauseAction.Get(); }
	UInputAction* GetContinueAction() const { return ContinueAction.Get(); }
	UInputAction* GetTogglePauseAction() const { return TogglePauseAction.Get(); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> MoveHorizontallyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> PauseAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ContinueAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> TogglePauseAction;
};