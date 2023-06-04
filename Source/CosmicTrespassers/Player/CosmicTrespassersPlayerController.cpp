// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersPlayerController.h"
#include "CosmicTrespassersCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionConfig.h"
#include "Common/CommonAttributeSet.h"
#include "GameplayAbilities/CosmicTrespassersAbilitySystemComponent.h"
#include "Framework/CosmicTrespassersGameInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void ACosmicTrespassersPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(false);
}

void ACosmicTrespassersPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UCosmicTrespassersGameInstance& GameInstance = UCosmicTrespassersGameInstance::Get();
	
	GameInstance.SetPlayerController(this);
	GameInstance.GetSubsystem<UGameplayMessageSubsystem>()->RegisterListener(TAG_Message_Channel_Game_Phase, this, &ThisClass::OnGamePhaseMessageReceived);

	CosmicTrespassersCharacter = Cast<ACosmicTrespassersCharacter>(InPawn);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(InputActionConfig->GetInputMappingContext(), 0);
	
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(CosmicTrespassersCharacter->InputComponent.Get());

	auto* AbilitySystem = Cast<UCosmicTrespassersAbilitySystemComponent>(CosmicTrespassersCharacter->GetAbilitySystemComponent());
	const auto* CommonAttributeSet = Cast<UCommonAttributeSet>(AbilitySystem->GetAttributeSet(UCommonAttributeSet::StaticClass()));

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(CommonAttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACosmicTrespassersPlayerController::OnGamePhaseMessageReceived(FGameplayTag, const FGamePhaseMessage& Message)
{
	if (Message.GamePhaseEventTag.MatchesTagExact(TAG_Game_Phase_Entered))
	{
		OnGamePhaseEntered(Message.GamePhaseTag);
	}
}

void ACosmicTrespassersPlayerController::OnGamePhaseEntered(const FGameplayTag GamePhaseTag)
{
	if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_StartMenu))
	{
		AddStartMenuInputBindings();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_SpawnEnemies))
	{
		ClearInputBindings();
		AddPauseInputBinding();	
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Gameplay))
	{
		AddGameplayInputBindings();
	}
	else if (GamePhaseTag.MatchesTagExact(TAG_Game_Phase_Game_Over))
	{
		ClearInputBindings();
		AddGameOverInputBindings();
	} 
}

void ACosmicTrespassersPlayerController::AddStartMenuInputBindings()
{
	EnhancedInputComponent->BindAction(InputActionConfig->GetContinueAction(), ETriggerEvent::Triggered, this,		
		&ACosmicTrespassersPlayerController::OnContinueKeyPressed);
}

void ACosmicTrespassersPlayerController::AddGameplayInputBindings()
{		
	EnhancedInputComponent->BindAction(InputActionConfig->GetMoveHorizontallyAction(), ETriggerEvent::Triggered, this,		
		&ACosmicTrespassersPlayerController::AddHorizontalInput);

	EnhancedInputComponent->BindAction(InputActionConfig->GetFireAction(), ETriggerEvent::Triggered, this,
		&ACosmicTrespassersPlayerController::OnFireButtonPressed);	
}

void ACosmicTrespassersPlayerController::AddGameOverInputBindings()
{	
	EnhancedInputComponent->BindAction(InputActionConfig->GetContinueAction(), ETriggerEvent::Triggered, this,		
		&ACosmicTrespassersPlayerController::OnContinueKeyPressed);
}

void ACosmicTrespassersPlayerController::AddPauseInputBinding()
{	
	EnhancedInputComponent->BindAction(InputActionConfig->GetTogglePauseAction(), ETriggerEvent::Triggered, this,
		&ACosmicTrespassersPlayerController::OnTogglePauseButtonPressed);
}

void ACosmicTrespassersPlayerController::ClearInputBindings() const
{
	EnhancedInputComponent->ClearActionBindings();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACosmicTrespassersPlayerController::AddHorizontalInput(const FInputActionValue& InputActionValue)
{	
	const FVector InputVector = FVector(0, InputActionValue.Get<float>(), 0);	
	CosmicTrespassersCharacter->MoveHorizontally(InputVector);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACosmicTrespassersPlayerController::OnFireButtonPressed()
{
	Cast<UCosmicTrespassersAbilitySystemComponent>(CosmicTrespassersCharacter->GetAbilitySystemComponent())->TryActivateAbilityByTag(TAG_Ship_Ability_Attack);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ACosmicTrespassersPlayerController::OnTogglePauseButtonPressed()
{
	UCosmicTrespassersGameInstance::Get().TogglePause();
}

void ACosmicTrespassersPlayerController::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.NewValue >= ChangeData.OldValue)
	{
		return;
	}

	PlayDamageCameraShake();
}

void ACosmicTrespassersPlayerController::PlayDamageCameraShake()
{
	ClientStartCameraShake(DamageCameraShake);
}