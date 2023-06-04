// Copyright Alexander Terentyev. All Rights Reserved.

#include "GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "Framework/CosmicTrespassersPlayerState.h"
#include "Messages/MessageBlueprintFunctionLibrary.h"
#include "Player/CosmicTrespassersPlayerController.h"
#include "SaveSystem/SaveSubsystem.h"
#include "Score/ScoreSubsystem.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// First value in the tuple is achieved score, second value is whether this is a new high score
	TTuple<uint32, bool> ScoreData = GetGameInstance()->GetSubsystem<UScoreSubsystem>()->GetScoreData();

	const uint32 HiScore = ScoreData.Get<0>();
	
	if (ScoreData.Get<1>())
	{
		NewHiScore->SetVisibility(ESlateVisibility::Visible);

		GetGameInstance()->GetSubsystem<USaveSubsystem>()->SaveHiScore(HiScore);
	}
	else
	{
		NewHiScore->SetVisibility(ESlateVisibility::Hidden);
	}
	
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.UseGrouping = false;
	Score->SetText(FText::AsNumber(HiScore, &FormattingOptions));

	const int32 AccuracyValue = UCosmicTrespassersGameInstance::Get().GetPlayerState<ACosmicTrespassersPlayerState>()->CalculateAccuracy();
	const FString AccuracyString = FString::Printf(TEXT("%d PERCENT"), AccuracyValue);	 
	Accuracy->SetText(FText::FromString(AccuracyString));

	auto* PlayerController = UCosmicTrespassersGameInstance::Get().GetPlayerController<ACosmicTrespassersPlayerController>();
	PlayerController->OnContinuePressed.AddDynamic(this, &ThisClass::OnKeyPressed);

	PlayAnimation(PressSpaceScale, 0, 0);
}

void UGameOverWidget::NativeDestruct()
{
	Super::NativeDestruct();

	auto* PlayerController = UCosmicTrespassersGameInstance::Get().GetPlayerController<ACosmicTrespassersPlayerController>();
	PlayerController->OnContinuePressed.RemoveAll(this);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UGameOverWidget::OnKeyPressed()
{
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Exited, TAG_Game_Phase_Game_Over);
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_SpawnEnemies);
}