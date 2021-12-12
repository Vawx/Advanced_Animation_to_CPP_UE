#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DexProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPrimitiveComponent;
class UParticleSystemComponent;
class ADexImpact;

UCLASS()
class DEX_API ADexProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	ADexCharacterBase* OwningCharacter;
	
	void Init();	

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dex Projectile")
	USphereComponent* Collider;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dex Projectile")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dex Projectile")
	UParticleSystemComponent* FlightSystem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")		
	TSubclassOf<ADexImpact> ImpactTemplate;

	ADexImpact* Impact;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()	
	void OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	int Count;
	
};
