

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Character/DexCharacterBase.h"
#include "DexNotify_GroundedEntryState.generated.h"

UCLASS()
class DEX_API UDexNotify_GroundedEntryState : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_GroundedEntryState")
	ECharacterGroundedEntryState GroundedEntryState;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
