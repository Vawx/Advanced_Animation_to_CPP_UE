

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/DexCharacterBase.h"
#include "DexNotify_MovementAction.generated.h"

UCLASS()
class DEX_API UDexNotify_MovementAction : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "MovementAction")
	ECharacterMovementAction MovementAction;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const;
};
