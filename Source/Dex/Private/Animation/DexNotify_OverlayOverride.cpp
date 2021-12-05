

#include "Animation/DexNotify_OverlayOverride.h"
#include "Character/DexCharacterBase.h"
#include "Character/DexCharacterAnimInst.h"
#include "Components/SkeletalMeshComponent.h"

UDexNotify_OverlayOverride::UDexNotify_OverlayOverride(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	
}

void UDexNotify_OverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}
	
	if (ADexCharacterBase* DexCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		if (UDexCharacterAnimInst* AnimInstance = Cast<UDexCharacterAnimInst>(MeshComp->GetAnimInstance()))
		{
			AnimInstance->SetOverrideState(OverlayState);
		}
	}
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);	
}

void UDexNotify_OverlayOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}
	
	if (ADexCharacterBase* DexCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		if (UDexCharacterAnimInst* AnimInstance = Cast<UDexCharacterAnimInst>(MeshComp->GetAnimInstance()))
		{
			AnimInstance->SetOverrideState(0);
		}
	}
	
	Super::NotifyEnd(MeshComp, Animation);	
}

FString UDexNotify_OverlayOverride::GetNotifyName_Implementation() const
{
	FString NotifyName = FString(TEXT("OverlayState: "));
	NotifyName.Append(FString::FromInt(OverlayState));
	return NotifyName;
}