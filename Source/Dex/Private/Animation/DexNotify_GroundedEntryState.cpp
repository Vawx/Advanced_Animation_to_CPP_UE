

#include "Animation/DexNotify_GroundedEntryState.h"
#include "Camera/DexPlayerCameraAnimInst.h"
#include "Character/DexCharacterAnimInst.h"

UDexNotify_GroundedEntryState::UDexNotify_GroundedEntryState(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UDexNotify_GroundedEntryState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (UDexCharacterAnimInst* AnimInstance = Cast<UDexCharacterAnimInst>(MeshComp->GetAnimInstance()))
	{
		AnimInstance->GroundedEntryState = GroundedEntryState;
	}
}