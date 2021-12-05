

#include "Animation/DexNotify_EarlyBlendOut.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UDexNotify_EarlyBlendOut::UDexNotify_EarlyBlendOut(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}
void UDexNotify_EarlyBlendOut::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
}

void UDexNotify_EarlyBlendOut::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float DeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, DeltaTime);

	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}
	
	if (Montage)
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (ADexCharacterBase* OwningCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
			{
				if (bCheckMovementState && OwningCharacter->MovementState == MovementStateEquals)
				{
					AnimInstance->Montage_Stop(BlendOutTime, Montage);
				}
				
				if (bCheckStance && OwningCharacter->CharacterStance == StanceEquals)
				{
					AnimInstance->Montage_Stop(BlendOutTime, Montage);					
				}
				
				if (bCheckMovementInput && OwningCharacter->bHasMovementInput)
				{
					AnimInstance->Montage_Stop(BlendOutTime, Montage);					
				}
			}
		}
	}
}

FString UDexNotify_EarlyBlendOut::GetNotifyName_Implementation() const
{
	return TEXT("Early Blend Out");
}
