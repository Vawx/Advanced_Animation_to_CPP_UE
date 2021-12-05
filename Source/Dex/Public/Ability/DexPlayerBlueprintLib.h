#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DexPlayerBlueprintLib.generated.h"

class ADexCharacterBase;

UCLASS()
class DEX_API UDexPlayerBlueprintLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Dex Character")
	static bool AttemptRoll(ADexCharacterBase* Base);
	
};
