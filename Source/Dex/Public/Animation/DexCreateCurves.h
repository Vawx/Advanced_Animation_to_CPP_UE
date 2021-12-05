

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "DexCreateCurves.generated.h"

USTRUCT(BlueprintType)
struct FAnimCurveCreationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimCurveCreationData")
	int FrameNumber;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimCurveCreationData")
	float CurveValue;
};

USTRUCT(BlueprintType)
struct FAnimCurveCreationParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimCurveCreationParams")
	FName CurveName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimCurveCreationParams")
	bool bKeyEachFrame;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimCurveCreationParams")
	TArray<FAnimCurveCreationData> Keys;
};

UCLASS()
class DEX_API UDexCreateCurves : public UAnimationModifier
{
	GENERATED_BODY()

public:

	UDexCreateCurves();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCreateCurves")
	TArray<FAnimCurveCreationParams> CurvesToCreate;

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;	
};
