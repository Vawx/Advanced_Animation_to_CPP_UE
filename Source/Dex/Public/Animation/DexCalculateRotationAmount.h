

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifiers/Public/AnimationModifier.h"
#include "DexCalculateRotationAmount.generated.h"

UCLASS()
class DEX_API UDexCalculateRotationAmount : public UAnimationModifier
{
	GENERATED_BODY()

public:

	UDexCalculateRotationAmount() {}

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCalculateRotationAmount")
	FName CurveName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCalculateRotationAmount")
	FName RootBoneName;

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	
};
