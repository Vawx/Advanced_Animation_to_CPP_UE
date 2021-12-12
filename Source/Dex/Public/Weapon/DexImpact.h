#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DexImpact.generated.h"

DECLARE_DELEGATE_OneParam(FDexImpactOverlappedActors, TArray<AActor*> /*OverlappedActors*/)

class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class DEX_API ADexImpact : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	FTransform ImpactTransform;
	FDexImpactOverlappedActors& GetImpactOverlappedActors() { return ImpactOverlappedActors; }
	
	void Trigger();
	
protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	UParticleSystem* ImpactSystemTemplate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	USphereComponent* ImpactCollider;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	FDexImpactOverlappedActors ImpactOverlappedActors;	

	int Count;
};
