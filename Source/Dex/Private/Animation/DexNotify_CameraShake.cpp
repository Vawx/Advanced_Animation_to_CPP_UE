

#include "Animation/DexNotify_CameraShake.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraShakeBase.h"

UDexNotify_CameraShake::UDexNotify_CameraShake(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UDexNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && Animation && ShakeClass)
	{
		if (APawn* OwningPawn = Cast<APawn>(MeshComp->GetOwner()))
		{
			if (APlayerController* OwningController = Cast<APlayerController>(OwningPawn->GetController()))
			{
				OwningController->ClientStartCameraShake(ShakeClass, Scale);
			}
		}
	}
}