#include "Animation/DexNotify_Footstep.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

UDexNotify_Footstep::UDexNotify_Footstep(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UDexNotify_Footstep::PlaySoundAttached()
{
	if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
	{
		float FootstepMask = (bOverrideMaskCurve) ? VolumeMultiplier : (1.f - AnimInstance->GetCurveValue(TEXT("Mask_FootstepSound"))) * VolumeMultiplier;
		USoundBase* Sound = SoundAsset;
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAttached(Sound, MeshComponent, AttachPointName, FVector::ZeroVector, EAttachLocation::Type::SnapToTarget, true, FootstepMask, PitchMultiplier);
		if (AudioComp)
		{
			AudioComp->SetIntParameter(TEXT("FootstepType"), static_cast<int>(FootstepType));
		}
	}

	MeshComponent = nullptr;
	AnimSequenceBase = nullptr;
}

void UDexNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}

	MeshComponent = MeshComp;
	AnimSequenceBase = Animation;
	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		if (USoundBase* SoundBase = SoundAsset)
		{
			PlaySoundAttached();
		}
	}
}

