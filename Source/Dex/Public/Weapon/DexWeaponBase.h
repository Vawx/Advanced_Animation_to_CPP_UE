#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DexWeaponBase.generated.h"

UCLASS()
class DEX_API ADexWeaponBase : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
