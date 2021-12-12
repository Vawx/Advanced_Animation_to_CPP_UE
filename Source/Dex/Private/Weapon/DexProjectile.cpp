
#include "Weapon/DexProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADexProjectile::ADexProjectile(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetSphereRadius(16.f);
	SetRootComponent(Collider);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	InitialLifeSpan = 5.f;
}

void ADexProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Collider)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ADexProjectile::OnProjectileOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &ADexProjectile::OnProjectileEndOverlap);
	}
	
}


void ADexProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ADexProjectile::OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ADexProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADexProjectile::Destroyed()
{
	Collider->OnComponentBeginOverlap.RemoveAll(this);
	Collider->OnComponentEndOverlap.RemoveAll(this);
	Super::Destroyed();
}

