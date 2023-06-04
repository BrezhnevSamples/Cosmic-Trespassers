// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LifeWidget.generated.h"

UCLASS()
class COSMICTRESPASSERS_API ULifeWidget : public UUserWidget
{
	GENERATED_BODY()

	//~=============================================================================
	// Textures
public:
	bool IsFull() const { return bIsFull; }
	
private:
	bool bIsFull = true;

	//~=============================================================================
	// Animations
public:	
	void PlayFillLifeAnimation();
	void PlayLoseLifeAnimation();
	
protected:	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FillLife;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LoseLife;
};