
#include "Animation/DexNotify_ApplyTagRange.h"
#include "Character/DexCharacterBase.h"
#include "AbilitySystemComponent.h"

UDexNotify_ApplyTagRange::UDexNotify_ApplyTagRange(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

void UDexNotify_ApplyTagRange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (ADexCharacterBase* OwningCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = OwningCharacter->GetAbilityComponent<UAbilitySystemComponent>())
		{
			AbilitySystemComponent->AddLooseGameplayTag(TagToApply);
		}
	}
}

void UDexNotify_ApplyTagRange::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (ADexCharacterBase* OwningCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = OwningCharacter->GetAbilityComponent<UAbilitySystemComponent>())
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(TagToApply);
		}
	}
}

