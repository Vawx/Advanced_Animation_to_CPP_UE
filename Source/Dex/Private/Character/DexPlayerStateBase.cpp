
#include "Character/DexPlayerStateBase.h"
#include "Ability/DexAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"

ADexPlayerStateBase::ADexPlayerStateBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDexAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* ADexPlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}