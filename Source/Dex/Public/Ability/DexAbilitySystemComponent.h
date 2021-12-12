#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DexAbilitySystemComponent.generated.h"

class UCanvas;

struct FStringAndHeight
{
public:
	FStringAndHeight(const FString& InS, float InH) : S(InS), H(InH) {}
	FString S;
	float H;
};

struct FDrawDebugTags
{
public:
	FDrawDebugTags() {}
	static void DrawDebugTags(UCanvas* Canvas, AActor* Actor, UAbilitySystemComponent* Comp, const FVector& CameraPos, const FVector& CameraDir, float& VerticalOffset);
};

UCLASS()
class DEX_API UDexAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_UCLASS_BODY()

public:

	static void DrawAbilityTags(UCanvas* Canvas, APlayerController* PC);

};