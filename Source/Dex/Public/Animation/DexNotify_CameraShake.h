

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DexNotify_CameraShake.generated.h"

class UCameraShakeBase;

UCLASS()
class DEX_API UDexNotify_CameraShake : public UAnimNotify
{
	GENERATED_UCLASS_BODY()
	
protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexNotify_CameraShake")
	float Scale;
};
