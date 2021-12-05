#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability/DexGameplayAbility.h"
#include "DexAbilitySet.generated.h"

UENUM(BlueprintType)
namespace EAbilityInputBinds
{
	enum Type
	{
		AbilityOne UMETA(DisplayName = "Ability One"),  
		AbilityTwo UMETA(DisplayName = "Ability Two"),
		AbilityThree UMETA(DisplayName = "Ability Three"),
		AbilityFour UMETA(DisplayName = "Ability Four"),
		AbilityFive UMETA(DisplayName = "Ability Five"),
	};
}

USTRUCT( )
struct FAbilityBindInfo
{
	GENERATED_USTRUCT_BODY( )
	
	UPROPERTY(EditAnywhere, Category = "Ability Bind Info")
	TEnumAsByte<EAbilityInputBinds::Type> Command;
	
	UPROPERTY(EditAnywhere, Category = "Ability Bind Info")
	TSubclassOf<UDexGameplayAbility> GameplayAbilityClass;
};

UCLASS()
class DEX_API UDexAbilitySet : public UDataAsset
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Ability Set")
	TArray<FAbilityBindInfo> Abilities;
	
	void GiveAbilities(class UAbilitySystemComponent *AbilitySystemComponent) const;
};