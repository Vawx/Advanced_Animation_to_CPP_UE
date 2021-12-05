

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "DexCreateLayeringCurves.generated.h"

UCLASS()
class DEX_API UDexCreateLayeringCurves : public UAnimationModifier
{
	GENERATED_BODY()
	
public:

	UDexCreateLayeringCurves();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCreateLayeringCurves")
	TArray<FName> CurvesToCreate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCreateLayeringCurves")
	float DefaultValue;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCreateLayeringCurves")
	bool bKeyEachFrame;

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	
};
