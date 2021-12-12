#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/DexCharacterBase.h"
#include "Engine/DataTable.h"
#include "Weapon/DexProjectile.h"
#include "DexWeaponBase.generated.h"

#define DEBUG_DEXWEAPONBASE_PROJECTILE_TRACE 1

class UDexWeaponComponent;
class USkeletalMeshComponent;
class UParticleSystemComponent;

USTRUCT(BlueprintType)
struct FDexWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon Data")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon Data")
	TSubclassOf<ADexProjectile> ProjectileTemplate;

};

UCLASS()
class DEX_API ADexWeaponBase : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	void Fire();
	void AttachWeaponTo(ADexCharacterBase* Character);
	void DetachWeapon();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	ECharacterOverlayState Overlay;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")	
	FName AttachSocket;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")	
	FTransform GunAttachOffset;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon")
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dex Projectile")
	UParticleSystemComponent* FlashSystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon")
	UDexWeaponComponent* WeaponComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")	
	TSubclassOf<ADexProjectile> ProjectileTemplate;

	ADexCharacterBase* AttachedOwner;
};
