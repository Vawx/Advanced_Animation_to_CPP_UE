
#include "Weapon/DexWeaponBase.h"
#include "Weapon/DexWeaponComponent.h"
#include "Weapon/DexProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/DexPlayerControllerBase.h"
#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"

ADexWeaponBase::ADexWeaponBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(GetRootComponent());

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SceneRoot);

	WeaponComponent = CreateDefaultSubobject<UDexWeaponComponent>(TEXT("WeaponComponent"));
}

void ADexWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADexWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADexWeaponBase::Fire()
{
	if (ProjectileTemplate && AttachedOwner)
	{
		if (UWorld* World = GetWorld())
		{
			FVector Loc = FVector::ZeroVector;
			FRotator Rot = FRotator::ZeroRotator;
			if (ADexPlayerControllerBase* PlayerController = Cast<ADexPlayerControllerBase>(AttachedOwner->GetController()))
			{	
				APlayerCameraManager* CM = PlayerController->PlayerCameraManager.Get();
				if (CM)
				{
					Loc = CM->GetCameraLocation();
					Rot = CM->GetCameraRotation();

					FVector Start = Loc;
					FVector End = Start + (Rot.Vector() * 65555.f);
					FHitResult TraceHit;
					
					FCollisionQueryParams TraceParams;
					TraceParams.AddIgnoredActor(this);
					TraceParams.AddIgnoredActor(AttachedOwner);			
					if (World->LineTraceSingleByChannel(TraceHit, Start, End, ECollisionChannel::ECC_WorldStatic, TraceParams))
					{
						FVector MuzzleLoc = MeshComp->GetSocketLocation(FName(TEXT("Muzzle")));
						
#if DEBUG_DEXWEAPONBASE_PROJECTILE_TRACE
						DrawDebugLine(World, Start, End, FColor::Red, false, 3.5f);
						DrawDebugLine(World, MuzzleLoc, TraceHit.ImpactPoint, FColor::Orange, false, 3.5f);
						
						DrawDebugSphere(World, TraceHit.ImpactPoint, 8.f, 12, FColor::Green, false, 3.5f);
						DrawDebugSphere(World, MuzzleLoc, 8.f, 12, FColor::Yellow, false, 3.5f);			
#endif
						FVector Dir = TraceHit.ImpactPoint - MuzzleLoc;
						Dir.Normalize();
						
						FTransform SpawnTransform = FTransform(Dir.Rotation(), MuzzleLoc);
						ADexProjectile* Proj = World->SpawnActorDeferred<ADexProjectile>(ProjectileTemplate, SpawnTransform, this, AttachedOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
						Proj->OwningCharacter = AttachedOwner;
						Proj->FinishSpawning(SpawnTransform);
						Proj->Init();
					}
				}
			}
		}
	}
}

void ADexWeaponBase::AttachWeaponTo(ADexCharacterBase* Character)
{
	if (Character)
	{
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
			Character->OverlayState = Overlay;
			AttachedOwner = Character;
		}
	}
}

void ADexWeaponBase::DetachWeapon()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(false, false);
	}

	if (AttachedOwner)
	{
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
	else
	{		
		Destroy();
	}
}

