#include "Ability/DexAbilitySet.h"
#include "AbilitySystemComponent.h"

UDexAbilitySet::UDexAbilitySet(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UDexAbilitySet::GiveAbilities(UAbilitySystemComponent *AbilitySystemComponent) const
{
	if (AbilitySystemComponent)
	{
		for (const FAbilityBindInfo &BindInfo : Abilities)
		{
			if (BindInfo.GameplayAbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbilityClass->GetDefaultObject<UDexGameplayAbility>(), 1, (int32)BindInfo.Command));
			}
		}
	}
}