
#include "Weapon/DexImpact.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"

ADexImpact::ADexImpact(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	ImpactCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ImpactCollider"));
	SetRootComponent(ImpactCollider);

	Count = 0;
}

void ADexImpact::BeginPlay()
{
	Super::BeginPlay();

	if (Count == 0)
	{
		
	}
}

void ADexImpact::Trigger()
{
	if (ImpactCollider)
	{
		TArray<AActor*> Overlapping;
		ImpactCollider->GetOverlappingActors(Overlapping);
		ImpactOverlappedActors.ExecuteIfBound(Overlapping);
	}
	
	if (ImpactSystemTemplate)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactSystemTemplate, GetActorLocation(), GetActorRotation());
	}
	
	SetLifeSpan(1.5f);
	Count++;
}

void ADexImpact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADexImpact::Destroyed()
{
	Super::Destroyed();
}

