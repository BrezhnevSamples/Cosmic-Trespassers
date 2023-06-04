// Copyright Alexander Terentyev. All Rights Reserved.

#include "HUDWidget.h"
#include "AbilitySystemComponent.h"
#include "LifeWidget.h"
#include "Components/TextBlock.h"
#include "Common/CommonAttributeSet.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Player/CosmicTrespassersCharacter.h"
#include "Score/ScoreSubsystem.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UScoreSubsystem* ScoreManager = GetGameInstance()->GetSubsystem<UScoreSubsystem>();

	UpdateScoreText(0, ScoreManager->GetCurrentScore());
	ScoreManager->OnScoreChanged().AddUObject(this, &UHUDWidget::UpdateScoreText);

	LiveWidgets.Emplace(FirstLife.Get());
	LiveWidgets.Emplace(SecondLife.Get());
	LiveWidgets.Emplace(ThirdLife.Get());

	auto* Character = UCosmicTrespassersGameInstance::Get().GetCharacter<ACosmicTrespassersCharacter>();
	
	auto* AttributeSet = Cast<UCommonAttributeSet>(Character->GetAbilitySystemComponent()->GetAttributeSet(UCommonAttributeSet::StaticClass()));
	MaxHealth = AttributeSet->GetMaxHealth();
	
	Character->OnHealthChanged().AddUObject(this, &UHUDWidget::UpdateLifeImages);	
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetGameInstance()->GetSubsystem<UScoreSubsystem>()->OnScoreChanged().RemoveAll(this);
	auto* Character = UCosmicTrespassersGameInstance::Get().GetCharacter<ACosmicTrespassersCharacter>();
	Character->OnHealthChanged().RemoveAll(this);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UHUDWidget::UpdateScoreText(const uint32, const uint32 Score) const
{
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.UseGrouping = false;
	ScoreText->SetText(FText::AsNumber(Score, &FormattingOptions));
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UHUDWidget::UpdateLifeImages(const float Health)
{	
	for (uint8 i = 0; i < MaxHealth; i++)
	{
		ULifeWidget* LifeWidget = LiveWidgets[i];
		
		if (Health > i)
		{
			if (LifeWidget->IsFull())
			{
				continue;
			}

			LifeWidget->PlayFillLifeAnimation();
		}
		else
		{
			if (!LifeWidget->IsFull())
			{
				continue;
			}

			LifeWidget->PlayLoseLifeAnimation();
		}			
	}
}