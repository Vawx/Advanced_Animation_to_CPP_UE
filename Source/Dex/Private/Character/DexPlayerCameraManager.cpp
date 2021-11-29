
#include "Character/DexPlayerCameraManager.h"
#include "Character/DexCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

ADexPlayerCameraManager::ADexPlayerCameraManager(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	CameraBehavior = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
	CameraBehavior->SetupAttachment(GetRootComponent());
}

void ADexPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();	
}

void ADexPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);
	
	if (const ADexCharacterBase* Character = GetControlledPawn<ADexCharacterBase>())
	{
		ensure(Character->GetController<APlayerController>());
		const FTransform PivotTarget = Character->GetActorTransform();
		const FVector FPTarget = Character->GetMesh() ? Character->GetMesh()->GetSocketLocation(TEXT("FP_Target")) : FVector::ZeroVector;
		const float ThirdPersonFOV = Character->ThirdPersonFOV;
		const float FirstPersonFOV = Character->FirstPersonFOV;
		TargetCameraRotation = FMath::RInterpTo(GetCameraRotation(), Character->GetController<APlayerController>()->GetControlRotation(), DeltaTime, GetCameraBehaviorParam(TEXT("RotationLagSpeed")));
		SmoothedPivotPoint.SetLocation(CalculateAxisIndependentLag(SmoothedPivotPoint.GetLocation(), PivotTarget.GetLocation(), FVector(GetCameraBehaviorParam(TEXT("PivotLagSpeed_X")), GetCameraBehaviorParam(TEXT("PivotLagSpeed_Y")), GetCameraBehaviorParam(TEXT("PivotLagSpeed_Z"))), TargetCameraRotation, DeltaTime));
		SmoothedPivotPoint.SetRotation(PivotTarget.GetRotation());
		PivotLocation = SmoothedPivotPoint.GetLocation() + (SmoothedPivotPoint.GetRotation().GetForwardVector() * GetCameraBehaviorParam(TEXT("PivotOffset_X"))) + (SmoothedPivotPoint.GetRotation().GetRightVector() * GetCameraBehaviorParam(TEXT("PivotOffset_Y"))) + (SmoothedPivotPoint.GetRotation().GetUpVector() * GetCameraBehaviorParam(TEXT("PivotOffset_Z")));
		TargetCameraLocation = PivotLocation + (TargetCameraRotation.Quaternion().GetForwardVector() * GetCameraBehaviorParam(TEXT("CameraOffset_X"))) + (TargetCameraRotation.Quaternion().GetRightVector() * GetCameraBehaviorParam(TEXT("CameraOffset_Y"))) + (TargetCameraRotation.Quaternion().GetUpVector() * GetCameraBehaviorParam(TEXT("CameraOffset_Z")));
		
		if (const UWorld* World = GetWorld())
		{
			FVector TraceOrigin = FVector::ZeroVector;
			float TraceRadius = 15.f;
			ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Camera;
			if (const USkeletalMeshComponent* CharacterMesh = Character->GetMesh())
			{
				TraceOrigin = Character->bRightShoulder ? CharacterMesh->GetSocketLocation(FName(TEXT("TP_CameraTrace_R"))) : CharacterMesh->GetSocketLocation(FName(TEXT("TP_CameraTrace_L")));
			}
			else
			{
				return;
			}

			FHitResult SphereHitResult;
			if (World->SweepSingleByChannel(SphereHitResult, TraceOrigin, TargetCameraLocation, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(TraceRadius)))
			{
				if (SphereHitResult.IsValidBlockingHit())
				{
					TargetCameraLocation = (SphereHitResult.Location - SphereHitResult.TraceEnd) + TargetCameraLocation;
				}
			}
		}
		
		const FTransform FinalA = FTransform(TargetCameraRotation, TargetCameraLocation);
		const FTransform FinalB = FTransform(TargetCameraRotation, FPTarget);
		const FTransform Final = UKismetMathLibrary::TLerp(FinalA, FinalB, GetCameraBehaviorParam(TEXT("Weight_FirstPerson")));
		OutVT.POV.Location = Final.GetLocation();
		OutVT.POV.Rotation = Final.GetRotation().Rotator();
		ensure(static_cast<int>(ThirdPersonFOV) && static_cast<int>(FirstPersonFOV));
		OutVT.POV.FOV = FMath::Lerp(ThirdPersonFOV, FirstPersonFOV, GetCameraBehaviorParam(TEXT("Weight_FirstPerson")));
	}
}

const FVector ADexPlayerCameraManager::CalculateAxisIndependentLag(const FVector& CurrentLocation, const FVector& TargetLocation, const FVector& LagSpeeds, const FRotator& CameraRotation, const float DeltaTime) const
{
	const FRotator CameraRotationYaw = FRotator(0.f, CameraRotation.Yaw, 0.f);
	const FVector A = CameraRotationYaw.UnrotateVector(CurrentLocation);
	const FVector B = CameraRotationYaw.UnrotateVector(TargetLocation);
	const float X = FMath::FInterpTo(A.X, B.X, DeltaTime, LagSpeeds.X);
	const float Y = FMath::FInterpTo(A.Y, B.Y, DeltaTime, LagSpeeds.Y);
	const float Z = FMath::FInterpTo(A.Z, B.Z, DeltaTime, LagSpeeds.Z);
	const FVector R = CameraRotationYaw.RotateVector(FVector(X, Y, Z));
	return R;	
}

const float ADexPlayerCameraManager::GetCameraBehaviorParam(const FName& CurveName) const
{
	if (CameraBehavior)
	{
		if (const UAnimInstance* AnimInstance = CameraBehavior->GetAnimInstance())
		{
			float R = AnimInstance->GetCurveValue(CurveName);
			return R;
		}
	}
	return 0.f;
}

