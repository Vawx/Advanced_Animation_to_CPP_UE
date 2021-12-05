#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DexAbilitySystemComponent.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "UObject/ObjectMacros.h"

#include "DexAbilitySystemInterface.generated.h"

template<class TClass> class TSubclassOf;

/** Interface for actors that expose the attribute set classes used */
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UDexAbilitySystemInterface : public UAbilitySystemInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IDexAbilitySystemInterface : public IAbilitySystemInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual UDexAbilitySystemComponent* GetDexAbilitySystemComponent() const { return Cast<UDexAbilitySystemComponent>(GetAbilitySystemComponent()); }
};