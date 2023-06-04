// Copyright Alexander Terentyev. All Rights Reserved.

#include "ShipAbility_Destroy.h"
#include "CosmicTrespassers/Common/HealthInterface.h"
#include "CosmicTrespassers/Framework/CosmicTrespassersGameInstance.h"
#include "CosmicTrespassers/Messages/MessageBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UShipAbility_Destroy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,	const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	//TODO: Play explosion effect and audio here

	AActor* Owner = ActorInfo->OwnerActor.Get();

	FOnAttributeChangeData HealthChangeData;
	HealthChangeData.OldValue = 1;
	HealthChangeData.NewValue = 0;
	
	// Animate the last life sprite
	Cast<IHealthInterface>(Owner)->OnHealthAttributeChanged(HealthChangeData);

	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);	
	
	UMessageBlueprintFunctionLibrary::BroadcastGamePhaseMessage(TAG_Game_Phase_Entered, TAG_Game_Phase_Game_Over);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
