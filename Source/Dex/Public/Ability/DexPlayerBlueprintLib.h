#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "DexPlayerBlueprintLib.generated.h"

class ADexCharacterBase;

UCLASS()
class DEX_API UDexPlayerBlueprintLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool AttemptRoll(ADexCharacterBase* Base);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static TArray<FGameplayTag> GetArrayFromContainer(FGameplayTagContainer Container);
	
};
