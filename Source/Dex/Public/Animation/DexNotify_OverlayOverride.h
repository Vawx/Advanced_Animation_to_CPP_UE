

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DexNotify_OverlayOverride.generated.h"

UCLASS()
class DEX_API UDexNotify_OverlayOverride : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

protected:

	/** Blending state for character animation instance, not related to Overlay state in Character */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "OverlayOverride")
	int OverlayState;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const;	
};
