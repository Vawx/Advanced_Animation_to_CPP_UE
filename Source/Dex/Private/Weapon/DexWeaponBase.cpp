
#include "Weapon/DexWeaponBase.h"
#include "Weapon/DexWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"

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

void ADexWeaponBase::AttachWeaponTo(ADexCharacterBase* Character)
{
	if (Character)
	{
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
			Character->OverlayState = Overlay;
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

