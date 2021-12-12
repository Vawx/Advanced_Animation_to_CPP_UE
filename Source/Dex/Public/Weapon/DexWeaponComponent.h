#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DexWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEX_API UDexWeaponComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
