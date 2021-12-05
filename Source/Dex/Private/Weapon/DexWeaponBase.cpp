
#include "Weapon/DexWeaponBase.h"

ADexWeaponBase::ADexWeaponBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADexWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADexWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

