#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Ability/DexAbilitySystemComponent.h"
#include "DexPlayerStateBase.generated.h"

class UDexAbilitySystemComponent;

UCLASS()
class DEX_API ADexPlayerStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_UCLASS_BODY()

public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ability")
	UDexAbilitySystemComponent* AbilitySystemComponent;
	
};
