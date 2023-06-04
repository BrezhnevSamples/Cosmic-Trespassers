// Copyright Alexander Terentyev. All Rights Reserved.

#include "StartMenuWidget.h"
#include "Components/TextBlock.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Player/CosmicTrespassersPlayerController.h"
#include "Score/ScoreSubsystem.h"

void UStartMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	
	auto* PlayerController = UCosmicTrespassersGameInstance::Get().GetPlayerController<ACosmicTrespassersPlayerController>();
	PlayerController->OnContinuePressed.AddDynamic(this, &ThisClass::OnSpacePressed);

	UpdateMaxScore(GetGameInstance()->GetSubsystem<UScoreSubsystem>()->GetHiScore());

	PlayAnimation(PressSpaceScale, 0, 0);
}

void UStartMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	auto* PlayerController = UCosmicTrespassersGameInstance::Get().GetPlayerController<ACosmicTrespassersPlayerController>();
	PlayerController->OnContinuePressed.RemoveAll(this);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UStartMenuWidget::OnSpacePressed()
{
	UCosmicTrespassersGameInstance::Get().StartGame();
}

void UStartMenuWidget::UpdateMaxScore(const uint32 Score) const
{
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.UseGrouping = false;
	HiScore->SetText(FText::AsNumber(Score, &FormattingOptions));
}