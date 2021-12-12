
#include "HUD/DexGameHUD.h"
#include "Ability/DexAbilitySystemComponent.h"

ADexGameHUD::ADexGameHUD(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	bDrawAbilityTags = true;
}

void ADexGameHUD::ToggleDebugAbiltyTags()
{
	bDrawAbilityTags = !bDrawAbilityTags;
}

void ADexGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawAbilityTags)
	{
		UDexAbilitySystemComponent::DrawAbilityTags(Canvas, PlayerOwner);
	}
}