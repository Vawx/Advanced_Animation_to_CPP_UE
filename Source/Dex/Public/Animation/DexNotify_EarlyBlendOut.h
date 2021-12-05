

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/DexCharacterBase.h"
#include "DexNotify_EarlyBlendOut.generated.h"

UCLASS()
class DEX_API UDexNotify_EarlyBlendOut : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

protected:

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float DeltaTime) override;
	FString GetNotifyName_Implementation() const;	
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	bool bCheckMovementState;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	bool bCheckStance;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	bool bCheckMovementInput;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	float BlendOutTime;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	ECharacterMovementState MovementStateEquals;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	ECharacterStance StanceEquals;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "DexNotify_EarlyBlendOut")
	UAnimMontage* Montage;

private:

};
