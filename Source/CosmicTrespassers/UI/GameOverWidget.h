// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UTextBlock;

UCLASS()
class COSMICTRESPASSERS_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnKeyPressed();
	
	//~=============================================================================
	// Score
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Score;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NewHiScore;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Accuracy;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PressSpaceScale;
};