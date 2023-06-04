// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SplashWidget.generated.h"
 
UCLASS()
class COSMICTRESPASSERS_API USplashWidget : public UUserWidget
{
	GENERATED_BODY()

	//~=============================================================================
	// Animations
protected:
	virtual void NativeConstruct() override;
	
	//~=============================================================================
	// Animations
protected:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EnemyHover;
};