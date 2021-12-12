#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "DexNotify_ApplyTagRange.generated.h"

UCLASS()
class DEX_API UDexNotify_ApplyTagRange : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Gameplay Tag")
	FGameplayTag TagToApply;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
};
