

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DexNotify_Footstep.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;
class USoundBase;

UENUM(BlueprintType)
enum class EFootstepType : uint8
{
	Step,
	WalkRun,
	Jump,
	Land
};

UCLASS()
class DEX_API UDexNotify_Footstep : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	
	

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	FName AttachPointName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	EFootstepType FootstepType;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	float VolumeMultiplier;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	float PitchMultiplier;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	bool bOverrideMaskCurve;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_Footstep")
	USoundBase* SoundAsset;

private:

	void PlaySoundAttached();

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY()
	UAnimSequenceBase* AnimSequenceBase;
};
