#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DexProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPrimitiveComponent;

UCLASS()
class DEX_API ADexProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dex Projectile")
	USphereComponent* Collider;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dex Projectile")
	UProjectileMovementComponent* MovementComponent;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
