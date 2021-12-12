#include "Weapon/DexWeaponComponent.h"

UDexWeaponComponent::UDexWeaponComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDexWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UDexWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

