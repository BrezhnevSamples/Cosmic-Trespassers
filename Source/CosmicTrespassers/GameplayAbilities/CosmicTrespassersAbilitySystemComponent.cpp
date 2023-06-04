// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersAbilitySystemComponent.h"
#include "Common/CommonAttributeSet.h"
#include "Common/HealthInterface.h"

void UCosmicTrespassersAbilitySystemComponent::ApplyInitEffect()
{
	ApplyEffectToSelf(InitAttributesEffect.GetDefaultObject(), 0);
}

void UCosmicTrespassersAbilitySystemComponent::Init()
{
	for (const TSubclassOf<UGameplayAbility> DefaultAbility : DefaultAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, INDEX_NONE, GetOwner()));
	}

	for (const TSubclassOf<UAttributeSet> AttributeSet : AttributeSets)
	{
		GetOrCreateAttributeSubobject(AttributeSet);
	}	
	
	auto* CommonAttributeSet = Cast<UCommonAttributeSet>(GetAttributeSet(UCommonAttributeSet::StaticClass()));
	
	ApplyInitEffect();
	
	GetGameplayAttributeValueChangeDelegate(CommonAttributeSet->GetCurrentHealthAttribute())
		.AddUObject(this, &UCosmicTrespassersAbilitySystemComponent::OnHealthAttributeChanged);
}

bool UCosmicTrespassersAbilitySystemComponent::HealthIsMax() const
{
	auto* CommonAttributeSet = Cast<UCommonAttributeSet>(GetAttributeSet(UCommonAttributeSet::StaticClass()));
	return CommonAttributeSet->HealthIsMax();
}

bool UCosmicTrespassersAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag Tag) 
{	
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(Tag);

	return TryActivateAbilitiesByTag(GameplayTagContainer);
}

void UCosmicTrespassersAbilitySystemComponent::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> Effect,	const int Level)
{
	const auto* GameplayEffect = Effect.GetDefaultObject();
	ApplyEffectToSelf(GameplayEffect, Level);
}

void UCosmicTrespassersAbilitySystemComponent::ApplyEffectToSelf(const UGameplayEffect* Effect, const int Level)
{
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ApplyGameplayEffectToSelf(Effect, Level, ContextHandle);
}

void UCosmicTrespassersAbilitySystemComponent::OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.NewValue == 0)
    {		
    	TryActivateAbilityByTag(DestroyAbilityTag);
    	return;
    }
    
    auto* HealthInterface = Cast<IHealthInterface>(GetOwner());

    if (HealthInterface == nullptr)
    {
    	return;
    }

    HealthInterface->OnHealthAttributeChanged(ChangeData);	
}
