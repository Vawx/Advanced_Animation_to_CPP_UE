#include "Ability/DexAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

UDexAbilitySet::UDexAbilitySet(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UDexAbilitySet::GiveAbilities(UAbilitySystemComponent *AbilitySystemComponent) const
{
	if (AbilitySystemComponent)
	{
		for (const FAbilityBindInfo &BindInfo : BindAbilities)
		{
			if (BindInfo.GameplayAbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbilityClass->GetDefaultObject<UDexGameplayAbility>(), 1, (int32)BindInfo.Command));
			}
		}

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : NonBindAbilties)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass));
			}
		}
	}
}