

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "DexCopyCurves.generated.h"

class UAnimSequence;

UCLASS()
class DEX_API UDexCopyCurves : public UAnimationModifier
{
	GENERATED_BODY()

public:

	UDexCopyCurves();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCopyCurves")
	UAnimSequence* AnimSequence;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCopyCurves")
	bool bCopyAllCurves;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCopyCurves")
	TArray<FName> CurvesToCopy;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DexCopyCurves")
	TArray<FName> AllCurves;

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

private:

	void Copy(TArray<FName> Curves);
	void OnApplyDelayed(UObject* Obj);
};
