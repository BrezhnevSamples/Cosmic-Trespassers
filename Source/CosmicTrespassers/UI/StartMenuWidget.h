// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "StartMenuWidget.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnSpacePressed();

	//~=============================================================================
	// Animations
protected:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PressSpaceScale;
	
	//~=============================================================================
	// Score
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HiScore;

private:
	UFUNCTION()
	void UpdateMaxScore(const uint32 Score) const;	
};