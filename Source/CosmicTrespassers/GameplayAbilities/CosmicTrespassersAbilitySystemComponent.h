// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CosmicTrespassersAbilitySystemComponent.generated.h"

DECLARE_EVENT(UCosmicTrespassersAbilitySystemComponent, FOnAbilitySystemInitialized)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COSMICTRESPASSERS_API UCosmicTrespassersAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	//~=============================================================================
	// Instance
public:
	void ApplyInitEffect();
	virtual void Init();
	
	//~=============================================================================
	// Attributes
public:
	bool HealthIsMax() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UAttributeSet>> AttributeSets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InitAttributesEffect;
	
	//~=============================================================================
	// Abilities
public:
	bool TryActivateAbilityByTag(const FGameplayTag Tag);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	//~=============================================================================
	// Effects	
public:	
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const int Level);
	void ApplyEffectToSelf(const UGameplayEffect* Effect, const int Level);

	//~=============================================================================
	// Health
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FGameplayTag DestroyAbilityTag;
	
private:
	void OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData);
};