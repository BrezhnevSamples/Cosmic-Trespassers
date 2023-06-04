// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UImage;
class UTexture2D;
class ULifeWidget;
class UPaperSprite;

UCLASS()
class COSMICTRESPASSERS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	//~=============================================================================
	// Score
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;	

private:	
	UFUNCTION()
	void UpdateScoreText(const uint32 AddedScore, const uint32 Score) const;

	//~=============================================================================
	// Lives
protected:	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULifeWidget> FirstLife;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULifeWidget> SecondLife;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULifeWidget> ThirdLife;

private:
	TArray<ULifeWidget*> LiveWidgets;

	uint8 MaxHealth;

	UFUNCTION()
	void UpdateLifeImages(const float Health);
};