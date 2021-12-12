
#include "Weapon/DexProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ADexProjectile::ADexProjectile(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetSphereRadius(16.f);
	SetRootComponent(Collider);

	FlightSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlightSystem"));
	FlightSystem->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	InitialLifeSpan = 5.f;

	Impact = nullptr;
}

void ADexProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ADexProjectile::Init()
{
	if (Collider)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ADexProjectile::OnProjectileOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &ADexProjectile::OnProjectileEndOverlap);
	}
	
	if (FlightSystem)
	{
		FlightSystem->Activate();
	}
}

void ADexProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ImpactTemplate && nullptr == Impact)
	{
		if (UWorld* World = GetWorld())
		{
			FTransform ImpactTransform = FTransform(SweepResult.ImpactNormal.Rotation(), SweepResult.ImpactPoint);
			Impact = World->SpawnActorDeferred<ADexImpact>(ImpactTemplate, ImpactTransform, this, OwningCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (Impact)
			{
				Impact->ImpactTransform = ImpactTransform;
				Impact->GetImpactOverlappedActors().BindUObject(OwningCharacter, &ADexCharacterBase::ImpactOverlap);
				Impact->FinishSpawning(ImpactTransform);
				Impact->Trigger();				

				Destroy();
			}
		}
	}
}

void ADexProjectile::OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	Collider->OnComponentEndOverlap.RemoveAll(this);
}

void ADexProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADexProjectile::Destroyed()
{
	Collider->OnComponentBeginOverlap.RemoveAll(this);
	Collider->OnComponentEndOverlap.RemoveAll(this);
	if (FlightSystem)
	{
		FlightSystem->DestroyComponent(false);
	}
	Super::Destroyed();
}

