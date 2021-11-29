
#include "Character/DexCharacterMovementData.h"
#include "Utility/DexMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveVector.h"
#include "Character/DexCharacterBase.h"
#include "Components/CapsuleComponent.h"

UDexCharacterMovementData::UDexCharacterMovementData()
{
	Gait = EMovementGait::Walking;
	DesiredGait = EMovementGait::Running;
	RotationMode = ERotationMode::LookingDirection;
	DesiredRotationMode = ERotationMode::LookingDirection;
	Stance = EStance::Standing;
	DesiredStance = EStance::Standing;
	MovementState = EMovementState::Grounded;
	PreviousMovementState = EMovementState::None;
	MovementAction = EMovementAction::None;
	ViewMode = EViewMode::ThirdPerson;
	OverlayState = EOverlayState::Default;
}

void UDexCharacterMovementData::Tick(float DeltaTime)
{
	if (ACharacter* Owning = GetOwningCharacter())
	{
		SetEssentialValues(Owning, DeltaTime);
		UpdateMovementState(Owning, DeltaTime);
	}
}

void UDexCharacterMovementData::PostInitProperties()
{
	Super::PostInitProperties();

	UObject* Owner = GetOuter();
	if (ensure(Owner))
	{
		CharacterOwner = Cast<ACharacter>(Owner);
	}
}

void UDexCharacterMovementData::SetEssentialValues(ACharacter* Character, const float DeltaTime)
{
	const FVector Vel = Character->GetVelocity();
	Acceleration = (Vel - PreviousVelocity) / DeltaTime;
	FVector VelocityLength = Vel;
	VelocityLength.Z = 0.f;
	Speed = VelocityLength.Size();
	bIsMoving = Speed > 1.f;
	if (bIsMoving)
	{
		LastVelocityRotation = Vel.Rotation();
	}
		
	if (const UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	{
		const FVector CurrentAcceleration = MovementComponent->GetCurrentAcceleration();
		MovementInputAmount = CurrentAcceleration.Size() / MovementComponent->GetMaxAcceleration();
		bHasMovementInput = MovementInputAmount > 0.f;
		if (bHasMovementInput)
		{
			LastMovementInputRotation = CurrentAcceleration.Rotation();
		}
	}		
		
	const FRotator ControllerRot = Character->GetControlRotation();
	const float Yaw = ControllerRot.Yaw;
	const float NewYaw = Yaw - PreviousAimYaw;
	const float Rate = NewYaw - DeltaTime;
	AimYawRate = FMath::Abs(Rate);

	PreviousVelocity = Vel;
	PreviousAimYaw = Yaw;
}

void UDexCharacterMovementData::UpdateMovementState(ACharacter* OwningCharacter, const float Delta)
{
	switch (MovementState)
	{
		case EMovementState::Grounded:
		{
			const EMovementGait AllowedGait = GetAllowedGait(OwningCharacter);
			const EMovementGait ActualGait = GetActualGait(AllowedGait);
			
			if (ActualGait != Gait)
			{
				Gait = ActualGait;
			}
			
			if (MovementModel)
			{
				UpdateDynamicMovementSettings(AllowedGait, OwningCharacter);
				UpdateGroundedRotation(OwningCharacter, Delta);
			}
		} break;
		case EMovementState::InAir:
		{
			UpdateInAirRotation(OwningCharacter, Delta);
			if (bHasMovementInput)
			{
				MantleCheck(OwningCharacter, FallingTraceSettings);
			}
		} break;
		case EMovementState::Ragdoll:
		{
			RagdollUpdate(OwningCharacter, Delta);
		} break;
	}
}

const EMovementGait UDexCharacterMovementData::GetAllowedGait(ACharacter* OwningCharacter) const
{
	auto CrouchingAimingGait = [](const EMovementGait G) {
		switch (G)
		{
			case EMovementGait::Walking:
			{
				return EMovementGait::Walking;
			} break;
			case EMovementGait::Running:
			case EMovementGait::Sprinting:
			{
				return EMovementGait::Running;
			} break;
		}
	};
	
	switch (Stance)
	{
		case EStance::Standing:
		{
			switch (RotationMode)
			{
				case ERotationMode::VelocityDirection:
				case ERotationMode::LookingDirection:
				{
					switch (DesiredGait)
					{
						case EMovementGait::Walking:
						{
							return EMovementGait::Walking;
						} break;
						case EMovementGait::Running:
						{
							return EMovementGait::Running;
						} break;
						case EMovementGait::Sprinting:
						{
							if (CanSprint(OwningCharacter)) 
							{
								return EMovementGait::Sprinting;								
							}
							else
							{
								return EMovementGait::Running;
							}
						} break;
					}
				} break;
				case ERotationMode::AimingDirection:
				{	
					return CrouchingAimingGait(DesiredGait);
				} break;
			}
		} break;
		case EStance::Crouching:
		{
			return CrouchingAimingGait(DesiredGait);
		} break;
	}
	
	ensure(!"Should never happen");
	return EMovementGait::None;
}

const bool UDexCharacterMovementData::CanSprint(ACharacter* OwningCharacter) const
{
	if (bHasMovementInput)
	{
		switch (RotationMode)
		{
			case ERotationMode::VelocityDirection:
			{
				return MovementInputAmount > 0.9f;
			} break;
			case ERotationMode::LookingDirection:
			{
				if (const UCharacterMovementComponent* MovementComp = OwningCharacter->GetCharacterMovement())
				{
					const FRotator AccelerationDirection = MovementComp->GetCurrentAcceleration().Rotation();
					const FRotator ControlRot = OwningCharacter->GetControlRotation();
					const FRotator Delta = (AccelerationDirection - ControlRot).GetNormalized();
					
					if (FMath::Abs(Delta.Yaw) < 50.f && MovementInputAmount > 0.9f)
					{
						return true;
					}
					return false;
				}
			} break;
			case ERotationMode::AimingDirection:
			{
				return false;
			} break;
		}
	}
	return false;
}

EMovementGait UDexCharacterMovementData::GetActualGait(const EMovementGait G)
{
	const float LocalWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocalRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;
	
	if (Speed >= LocalRunSpeed + 10.f)
	{
		switch (G)
		{
			case EMovementGait::Walking:
			case EMovementGait::Running:
			{
				return EMovementGait::Running;
			} break;
			case EMovementGait::Sprinting:
			{
				return EMovementGait::Sprinting;
			} break;
		}
	}
	
	if (Speed >= LocalWalkSpeed + 10.f)
	{
		return EMovementGait::Running;
	}
	return EMovementGait::Walking;
}

void UDexCharacterMovementData::UpdateDynamicMovementSettings(const EMovementGait G, ACharacter* OwningCharacter)
{
	if (UCharacterMovementComponent* MovementComp = OwningCharacter->GetCharacterMovement())
	{
		CurrentMovementSettings = GetTargetMovementSettings();
		
		switch (G)
		{
			case EMovementGait::Walking:
			{
				MovementComp->MaxWalkSpeed = CurrentMovementSettings.WalkSpeed;
			} break;
			case EMovementGait::Running:
			{
				MovementComp->MaxWalkSpeed = CurrentMovementSettings.RunSpeed;				
			} break;
			case EMovementGait::Sprinting:
			{
				MovementComp->MaxWalkSpeed = CurrentMovementSettings.SprintSpeed;				
			} break;
		}
		
		if (UCurveVector* Curve = CurrentMovementSettings.MovementCurve)
		{
			const float CurveTime = GetMappedSpeed();
			const FVector V = Curve->GetVectorValue(CurveTime);
			MovementComp->MaxAcceleration = V.X;
			MovementComp->BrakingDecelerationWalking = V.Y;
			MovementComp->GroundFriction = V.Z;
		}
	}
}

const FMovementSettings& UDexCharacterMovementData::GetTargetMovementSettings() const
{
	if (!MovementData)
	{
		static FMovementSettings T = {};
		return T;
	}
	
	switch (RotationMode)
	{
		case ERotationMode::VelocityDirection:
		{
			switch (Stance) 
			{
				case EStance::Standing:
				{
					return MovementData->VelocityDirection.Standing;
				} break;
				case EStance::Crouching:
				{
					return MovementData->VelocityDirection.Crouching;					
				} break;
			}
		} break;
		case ERotationMode::LookingDirection:
		{
			switch (Stance)
			{
				case EStance::Standing:
				{
					return MovementData->LookingDirection.Standing;
					
				} break;
				case EStance::Crouching:
				{
					return MovementData->LookingDirection.Crouching;
					
				} break;
			}
		} break;
		case ERotationMode::AimingDirection:
		{
			switch (Stance)
			{
				case EStance::Standing:
				{
					return MovementData->AimingDirection.Standing;
					
				} break;
				case EStance::Crouching:
				{
					return MovementData->AimingDirection.Crouching;
					
				} break;
			}
		}
	}
	ensure(!"Should never happen");
	return MovementData->VelocityDirection.Standing;
}

const float UDexCharacterMovementData::GetMappedSpeed() const
{
	const float LocalWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocalRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;
	const float Walk = DexMathUtility::RangeClamped(Speed, 0.f, LocalWalkSpeed, 0.f, 1.f);
	const float Run = DexMathUtility::RangeClamped(Speed, LocalWalkSpeed, LocalRunSpeed, 1.f, 2.f);
	const float Sprint = DexMathUtility::RangeClamped(Speed, LocalRunSpeed, LocalSprintSpeed, 2.f, 3.f);	
	const float A = Speed > LocalWalkSpeed ? Run : Walk;
	const float B = Speed > LocalRunSpeed ? Sprint : Run;
	return B;
}

void UDexCharacterMovementData::UpdateGroundedRotation(ACharacter* OwningCharacter, const float DeltaTime)
{
	switch (MovementAction)
	{
		case EMovementAction::None:
		{
			bool bCanUpdateMovingRotation = (bIsMoving && bHasMovementInput) || (Speed > 150.f && OwningCharacter->HasAnyRootMotion());
			if (bCanUpdateMovingRotation)
			{
				auto CalculateGroundedRotationRate = [&]()
				{
					if (const UCurveFloat* RotationCurve = CurrentMovementSettings.RotationRateCurve)
					{
						const float Mapped = RotationCurve->GetFloatValue(GetMappedSpeed());
						const float ClampedAimYaw = DexMathUtility::RangeClamped(AimYawRate, 0.f, 300.f, 1.f, 3.f);
						return Mapped * ClampedAimYaw;
					}
					return 0.f;
				};

				const FRotator ControlRotation = OwningCharacter->GetControlRotation();
				switch (RotationMode)
				{
					case ERotationMode::VelocityDirection:
					{
						SmoothCharacterRotation(OwningCharacter, FRotator(0.f, LastVelocityRotation.Yaw, 0.f), 800.f, CalculateGroundedRotationRate(), DeltaTime);
					} break;
					case ERotationMode::LookingDirection:
					{
						switch (Gait)
						{
							case EMovementGait::Walking:
							case EMovementGait::Running:
							{
								const float Yaw = ControlRotation.Yaw + GetAnimCurveValue(OwningCharacter, FName(TEXT("YawOffset")));
								SmoothCharacterRotation(OwningCharacter, FRotator(0.f, Yaw, 0.f), 500.f, CalculateGroundedRotationRate(), DeltaTime);
							} break;
							case EMovementGait::Sprinting:
							{
								SmoothCharacterRotation(OwningCharacter, FRotator(0.f, LastVelocityRotation.Yaw, 0.f), 500.f, CalculateGroundedRotationRate(), DeltaTime);
							}
						}
					} break;
					case ERotationMode::AimingDirection:
					{
						SmoothCharacterRotation(OwningCharacter, FRotator(0.f, ControlRotation.Yaw, 0.f), 1000.f, 20.f, DeltaTime);
					} break;
				}
			} 
			else
			{
				auto LimitRotationLambda = [&](const float Delta) {
					LimitRotation(OwningCharacter, -100.f, 100.f, 20.f, DeltaTime);
				};
				
				auto AddWorldRotationLambda = [&]() {
					const float RotAmount = GetAnimCurveValue(OwningCharacter, FName(TEXT("RotationAmount")));
					if (FMath::Abs(RotAmount) > 0.001f)
					{
						OwningCharacter->AddActorWorldRotation(FRotator(0.f, RotAmount * (DeltaTime / (1.f / 30.f)), 0.f));
						TargetRotation = OwningCharacter->GetActorRotation();
					}
				};
				
				switch (ViewMode)
				{
					case EViewMode::ThirdPerson:
					{
						switch (RotationMode)
						{
							case ERotationMode::VelocityDirection:
							case ERotationMode::LookingDirection:
							{
								AddWorldRotationLambda();
							} break;
							case ERotationMode::AimingDirection:
							{
								LimitRotationLambda(DeltaTime);
								AddWorldRotationLambda();
							} break;
						}
					} break;
					case EViewMode::FirstPerson:
					{
						LimitRotationLambda(DeltaTime);
						AddWorldRotationLambda();
					} break;
				}
			}
		} break;
		case EMovementAction::Rolling:
		{
			if (bHasMovementInput)
			{
				SmoothCharacterRotation(OwningCharacter, FRotator(0.f, LastMovementInputRotation.Yaw, 0.f), 0.f, 2.f, DeltaTime);
			}
		};
		default: {}
	}
}

void UDexCharacterMovementData::SmoothCharacterRotation(ACharacter* OwningPlayer, const FRotator& Target, const float TargetLerpSpeed, const float ActorInterpSpeed, const float DeltaTime)
{
	TargetRotation = FMath::RInterpTo(TargetRotation, Target, DeltaTime, TargetLerpSpeed);
	const FRotator Interp = FMath::RInterpTo(OwningPlayer->GetActorRotation(), TargetRotation, DeltaTime, ActorInterpSpeed);
	OwningPlayer->SetActorRotation(Interp);
}

const float UDexCharacterMovementData::GetAnimCurveValue(ACharacter* OwningCharacter, const FName& CurveName)
{
	float Result = 0.f;
	if (USkeletalMeshComponent* SkeletalMeshComponent = OwningCharacter->GetMesh())
	{
		if (const UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
		{
			Result = AnimInstance->GetCurveValue(CurveName);
		}
	}
	return Result;
}

void UDexCharacterMovementData::LimitRotation(ACharacter* OwningCharacter, const float AimYawMin, const float AimYawMax, const float InterpSpeed, const float DeltaTime)
{
	const FRotator ControlRot = OwningCharacter->GetControlRotation();
	FRotator Delta = DexMathUtility::NormalizedRotators(ControlRot, OwningCharacter->GetActorRotation());
	const float YawDelta = Delta.Yaw;
	const float ControlYaw = ControlRot.Yaw;
	
	if (!DexMathUtility::InRangeFloatFloat(YawDelta, AimYawMin, AimYawMax))
	{
		const float YawTarget = YawDelta > 0.f ? ControlYaw + AimYawMax : ControlYaw + AimYawMin;
		SmoothCharacterRotation(OwningCharacter, FRotator(0.f, YawTarget, 0.f), 0.f, InterpSpeed, DeltaTime);
	}
}

void UDexCharacterMovementData::UpdateInAirRotation(ACharacter* OwningCharacter, const float DeltaTime)
{
	switch (RotationMode)
	{
		case ERotationMode::VelocityDirection:
		case ERotationMode::LookingDirection:
		{
			SmoothCharacterRotation(OwningCharacter, FRotator(0.f, InAirRotation.Yaw, 0.f), 0.f, 5.f, DeltaTime);
		} break;
		case ERotationMode::AimingDirection:
		{
			SmoothCharacterRotation(OwningCharacter, FRotator(0.f, OwningCharacter->GetControlRotation().Yaw, 0.f), 0.f, 15.f, DeltaTime);		
			InAirRotation = OwningCharacter->GetActorRotation();
		} break;
	}
}

const bool UDexCharacterMovementData::MantleCheck(ACharacter* OwningCharacter, const FMantleTraceParams& TraceParams)
{
	FVector InitialTraceImpactPoint = FVector::ZeroVector;
	FVector InitialTraceNormal = FVector::ZeroVector;
	FVector DownTraceLocation = FVector::ZeroVector;
	UPrimitiveComponent* HitComponent = nullptr;
	FTransform TargetTransform = FTransform::Identity;
	float MantleHeight = 0.f;
	EMantleType MantleType = EMantleType::HighMantle;
	
	if (const UWorld* World = OwningCharacter->GetWorld())
	{
		if (const UCharacterMovementComponent* Movement = OwningCharacter->GetCharacterMovement())
		{
			const FVector PlayerMovementInput = GetPlayerMovementInput(OwningCharacter);
			FVector Start = GetCapsuleBaseLocation(OwningCharacter, 2.f) + (PlayerMovementInput * -30.f);
			Start.Z += (TraceParams.MaxLedgeHeight + TraceParams.MinLedgeHeight) / 2.f;
			FVector End = Start + (PlayerMovementInput * TraceParams.ReachDistance);
			
#if ENABLE_CHARACTER_DEBUG
			DrawDebugCapsule(World, Start, ((TraceParams.MaxLedgeHeight - TraceParams.MinLedgeHeight) / 2.f) + 1.f, TraceParams.ForwardTraceRadius, FQuat::Identity, FColor::Emerald, false, 8.f);
#endif
			FHitResult CapsuleResult;
			if (World->SweepSingleByChannel(CapsuleResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(TraceParams.ForwardTraceRadius, ((TraceParams.MaxLedgeHeight - TraceParams.MinLedgeHeight) / 2.f) + 1.f)))
			{
#if ENABLE_CHARACTER_DEBUG
				DrawDebugCapsule(World, CapsuleResult.ImpactPoint, ((TraceParams.MaxLedgeHeight - TraceParams.MinLedgeHeight) / 2.f) + 1.f, TraceParams.ForwardTraceRadius, FQuat::Identity, FColor::Magenta, false, 8.f);
#endif
				if (CapsuleResult.IsValidBlockingHit() && !Movement->IsWalkable(CapsuleResult))
				{
					InitialTraceImpactPoint = CapsuleResult.ImpactPoint;
					InitialTraceNormal = CapsuleResult.ImpactNormal;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			
			FVector ImpactOffset = FVector(InitialTraceImpactPoint.X, InitialTraceImpactPoint.Y, GetCapsuleBaseLocation(OwningCharacter, 2.f).Z);
			FVector ImpactOffsetMult = ImpactOffset + (InitialTraceNormal * -15.f);
			float ImpactZOverride = TraceParams.MaxLedgeHeight + TraceParams.DownwardTraceRadius + 1.f;
			Start = ImpactOffsetMult + FVector(0.f, 0.f, ImpactZOverride);
			End = ImpactOffsetMult;
			
#if ENABLE_CHARACTER_DEBUG
			DrawDebugSphere(World, Start, TraceParams.DownwardTraceRadius, 8, FColor::Red, false, 5.f);
#endif
			FHitResult SphereResult;
			if (World->SweepSingleByChannel(SphereResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(TraceParams.DownwardTraceRadius)))
			{
#if ENABLE_CHARACTER_DEBUG
				DrawDebugSphere(World, SphereResult.ImpactPoint, TraceParams.DownwardTraceRadius, 8, FColor::Yellow, false, 5.f);
#endif
				if (SphereResult.bBlockingHit && Movement->IsWalkable(SphereResult))
				{
					DownTraceLocation = SphereResult.ImpactPoint;
					HitComponent = Cast<UPrimitiveComponent>(SphereResult.GetComponent());
					AActor* HitActor = Cast<AActor>(SphereResult.GetActor());
				}
				else
				{
					return false;
				}
			}		
			else
			{
				return false;
			}
			
			UCapsuleComponent* CapsuleComp = OwningCharacter->GetCapsuleComponent();
			if (nullptr == CapsuleComp)
			{
				return false;
			}
			
			const FVector CapsuleLocationFromBase = GetCapsuleLocationFromBase(OwningCharacter, DownTraceLocation, 2.f);
#if ENABLE_CHARACTER_DEBUG
			DrawDebugBox(World, CapsuleLocationFromBase, FVector(32.f, 32.f, 32.f), FQuat::Identity, FColor::Black, false, 3.f);
#endif
			if (CapsuleHasRoomCheck(OwningCharacter, CapsuleComp, CapsuleLocationFromBase, 0.f, 0.f))
			{
				const FVector TransformDirection = InitialTraceNormal * FVector(-1.f, -1.f, 0.f);
				TargetTransform = FTransform(TransformDirection.Rotation(), CapsuleLocationFromBase);
				FVector MantleHeightVector = TargetTransform.GetLocation() - OwningCharacter->GetActorLocation();
				MantleHeight = MantleHeightVector.Z;
			}
			else
			{
				return false;
			}
			
			switch (MovementState)
			{
				case EMovementState::None:
				case EMovementState::Grounded:
				case EMovementState::Mantling:
				case EMovementState::Ragdoll:
				{
					MantleType = MantleHeight > 125.f ? EMantleType::HighMantle : EMantleType::LowMantle;
				} break;
				case EMovementState::InAir:
				{
					MantleType = EMantleType::FallingCatch;
				} break;
			}
			
			if (Mantle.Component.Get(false))
			{
				Mantle.Reset();
			}
			
			Mantle = FComponentAndTransform(TargetTransform, HitComponent);
			if (Mantle.Valid())
			{
				FVector Distance = TargetTransform.GetLocation() - OwningCharacter->GetActorLocation();
				float Length = Distance.Size();
				MantleStart(OwningCharacter, MantleHeight, Mantle, MantleType);
				return true;				
			}
		}	
	}
	return false;
}

const FVector UDexCharacterMovementData::GetPlayerMovementInput(ACharacter* OwningCharacter)
{
	FVector Result = FVector::ZeroVector;	
	if (ADexCharacterBase* DexCharacter = Cast<ADexCharacterBase>(OwningCharacter))
	{
		FVector Forward = FVector::ZeroVector;
		FVector Right = FVector::ZeroVector;
		const FRotator ControlRot = OwningCharacter->GetControlRotation();
		FVector ForwardVector = ControlRot.Quaternion().GetForwardVector();
		FVector RightVector = ControlRot.Quaternion().GetRightVector();

		FVector F = Forward * DexCharacter->CharacterRuntimeInputData.ForwardAxisAmount;
		FVector R = Right * DexCharacter->CharacterRuntimeInputData.RightAxisAmount;
		Result = (F + R).GetSafeNormal();
	}
	return Result;	
}

const FVector UDexCharacterMovementData::GetCapsuleBaseLocation(ACharacter* OwningCharacter, const float ZOffset)
{
	FVector Result = FVector::ZeroVector;
	if (const UCapsuleComponent* Capsule = OwningCharacter->GetCapsuleComponent())
	{
		FVector WorldLoc = Capsule->GetComponentLocation();
		FVector Up = Capsule->GetUpVector();
		float ScaledHalf = Capsule->GetScaledCapsuleHalfHeight();
		
		ScaledHalf += ZOffset;
		Up *= ScaledHalf;
		WorldLoc -= Up;
		Result = WorldLoc;
	}
	return Result;
}

const FVector UDexCharacterMovementData::GetCapsuleLocationFromBase(ACharacter* OwningCharacter, const FVector& BaseLocation, const float ZOffset)
{
	FVector Result = FVector::ZeroVector;
	if (const UCapsuleComponent* Capsule = OwningCharacter->GetCapsuleComponent())
	{
		Result = BaseLocation + FVector(0.f, 0.f, Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
	}
	return Result;
}

const bool UDexCharacterMovementData::CapsuleHasRoomCheck(ACharacter* OwningCharacter, const UCapsuleComponent* Capsule, const FVector& TargetLocation, const float HeightOffset, const float RadiusOffset)
{
	/** This function checks to see if _any other_ Character is around as the "check" */
	if (Capsule)
	{
		float CapsuleScaled = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere();
		float Z = CapsuleScaled + (RadiusOffset * -1.f) + HeightOffset;
		FVector Start = TargetLocation + FVector(0.f, 0.f, Z);
		FVector End = TargetLocation - FVector(0.f, 0.f, Z);
		float Rad = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;
		
		if (UWorld* World = OwningCharacter->GetWorld())
		{
#if ENABLE_CHARACTER_DEBUG
			DrawDebugCapsule(World, Start, CapsuleScaled, Rad, FQuat::Identity, FColor::Orange, false, 3.5f);
#endif
			FHitResult Sphere;
			FCollisionQueryParams Params;
			// Ignore OwningCharacter, only check for other "Character" profiles.
			Params.AddIgnoredActor(OwningCharacter);
			bool bHit = World->SweepSingleByProfile(Sphere, Start, End, FQuat::Identity, FName(TEXT("Character")), FCollisionShape::MakeSphere(Rad), Params);
#if ENABLE_CHARACTER_DEBUG
			if (bHit)
			{
				DrawDebugSphere(World, Sphere.ImpactPoint, 8, 8, FColor::Purple, false, 3.5f);
				DrawDebugLine(World, Start, End, FColor::Green, false, 5.f);
				DrawDebugLine(World, Start, Sphere.ImpactPoint, FColor::Red, false, 5.f);				
			}
#endif
			// both false (NOR)
			return !(Sphere.bBlockingHit || Sphere.bStartPenetrating);
		}
	}
	return false;
}

void UDexCharacterMovementData::MantleStart(ACharacter* OwningCharacter, const float MantleHeight, FComponentAndTransform& MantleLedgeWorldSpace, const EMantleType Type)
{
	const FMantleAsset& MantleAsset = GetMantleAsset(Type);
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingPosition = DexMathUtility::RangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowStartPosition, MantleAsset.HighStartPosition);
	MantleParams.PlayRate = DexMathUtility::RangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowPlayRate, MantleAsset.HighPlayRate);
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	
	MantleLedgeLS = DexMathUtility::ComponentWorldToLocal(MantleLedgeWorldSpace);
	MantleTarget = MantleLedgeWorldSpace.Transform;
	MantleActualStartOffset = DexMathUtility::TransformMinusTransform(OwningCharacter->GetActorTransform(), MantleTarget);
	
	FVector TargetLocation = MantleTarget.GetLocation();
	FVector TargetDirection = MantleTarget.GetRotation().Vector();
	FVector TargetDirectionOffset = TargetDirection * MantleParams.StartingOffset.Y;
	FVector TargetOffset = FVector(TargetDirectionOffset.X, TargetDirectionOffset.Y, MantleParams.StartingOffset.Z);
	FVector TargetFinal = TargetLocation - TargetOffset;
	FTransform A = FTransform(MantleTarget.GetRotation(), TargetFinal);
	MantleAnimationStartOffset = DexMathUtility::TransformMinusTransform(A, MantleTarget);
	
	if (UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement())
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
		MovementState = EMovementState::Mantling;
	}
	
	UCurveVector* PositionCorrectionCurve = MantleParams.PositionCorrectionCurve;														 
	if (PositionCorrectionCurve && !MantleTimeline.Active())
	{
		float Min = 0.f;
		float Max = 0.f;
		PositionCorrectionCurve->GetTimeRange(Min, Max);
		MantleTimeline.OnCharacterTimelineUpdate = FOnDexTimelineUpdate::CreateUObject(this, &UDexCharacterMovementData::MantleTimelineUpdate);
		MantleTimeline.OnCharacterTimelineFinished = FOnDexTimelineFinished::CreateUObject(this, &UDexCharacterMovementData::MantleTimelineFinished);
		MantleTimeline.SetPlayLength(Max - MantleParams.StartingPosition);
		MantleTimeline.Start(OwningCharacter, true);
	}
	
	UAnimMontage* ParamsMontage = MantleParams.AnimMontage;
	if (ParamsMontage)
	{
		if (USkeletalMeshComponent* MeshComp = OwningCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
			{
				AnimInstance->Montage_Play(ParamsMontage, 1.f, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition);
			}
		}
	}
}

const FMantleAsset& UDexCharacterMovementData::GetMantleAsset(const EMantleType Type)
{
	switch (Type)
	{
		case EMantleType::FallingCatch:
		case EMantleType::HighMantle:
		{
			return Mantle_2m_Default;
		} break;
		case EMantleType::LowMantle:
		{
			switch (OverlayState)
			{
				case EOverlayState::Default:
				case EOverlayState::Masculine:
				case EOverlayState::Feminine:
				{
					return Mantle_1m_Default;
				} break;
				case EOverlayState::Injured:
				case EOverlayState::Bow:
				case EOverlayState::Torch:
				case EOverlayState::Barrel:
				{
					return Mantle_1m_LH;
				} break;
				case EOverlayState::Rifle:
				case EOverlayState::PistolOneHand:
				case EOverlayState::PistolTwoHand:
				{
					return Mantle_1m_RH;
				} break;
				case EOverlayState::Box:
				{
					return Mantle_1m_Box;
				} break;
				case EOverlayState::HandsTied:
				{
					return Mantle_1m_2H;
				} break;
			}
		} break;
	}
	ensure("this shouldnt happen");
	return CharacterMantleAsset;
}

void UDexCharacterMovementData::MantleTimelineUpdate(float Delta)
{
	ACharacter *Owning = GetOwningCharacter();
	if (ensure(Owning))
	{
		MantleUpdate(Owning, Delta);
	}
}

void UDexCharacterMovementData::MantleTimelineFinished()
{
	ACharacter *Owning = GetOwningCharacter();
	if (ensure(Owning))
	{
		MantleEnd(Owning);
	}
}

void UDexCharacterMovementData::MantleEnd(ACharacter* OwningCharacter)
{
	if (UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement())
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking, 0);
		
		Mantle.Reset();
		MantleLedgeLS.Reset();
	}
}

void UDexCharacterMovementData::MantleUpdate(ACharacter* OwningCharacter, const float DeltaTime)
{
	float PositionAlpha = 0.f;
	float XYCorrectionAlpha = 0.f;
	float ZCorrectionAlpha = 0.f;
	FTransform LerpedTarget = FTransform::Identity;
	
	FComponentAndTransform CAT = DexMathUtility::ComponentLocalToWorld(MantleLedgeLS);
	MantleTarget = CAT.Transform;
	
	AActor* MantleLedgeLSOwner = MantleLedgeLS.Component->GetOwner();
	const FString Label = MantleLedgeLSOwner ? MantleLedgeLSOwner->GetActorLabel() : FString(TEXT("NONE"));
	
	UCurveVector* PositionCorrectionCurve = MantleParams.PositionCorrectionCurve;
	if (PositionCorrectionCurve)
	{
		const FVector PositionCorrectionCurveValue = PositionCorrectionCurve->GetVectorValue(MantleParams.StartingPosition + MantleTimeline.GetPlaybackValue());
		PositionAlpha = PositionCorrectionCurveValue.X;
		XYCorrectionAlpha = PositionCorrectionCurveValue.Y;
		ZCorrectionAlpha = PositionCorrectionCurveValue.Z;
		
		{
			FTransform A = FTransform::Identity;
			A.SetLocation(FVector(MantleAnimationStartOffset.GetLocation().X, MantleAnimationStartOffset.GetLocation().Y, MantleActualStartOffset.GetLocation().Z));
			A.SetRotation(MantleAnimationStartOffset.GetRotation());
			A.SetScale3D(FVector::OneVector);
			FTransform ALerp = DexMathUtility::TransformLerp(MantleActualStartOffset, A, XYCorrectionAlpha);
			
			FTransform B = FTransform::Identity;
			B.SetLocation(FVector(MantleActualStartOffset.GetLocation().X, MantleActualStartOffset.GetLocation().Y, MantleAnimationStartOffset.GetLocation().Z));
			B.SetRotation(MantleAnimationStartOffset.GetRotation());
			B.SetScale3D(FVector::OneVector);
			FTransform BLerp = DexMathUtility::TransformLerp(MantleActualStartOffset, B, ZCorrectionAlpha);
			
			FTransform C = FTransform::Identity;
			C.SetLocation(FVector(ALerp.GetLocation().X, ALerp.GetLocation().Y, BLerp.GetLocation().Z));
			C.SetRotation(ALerp.GetRotation());
			C.SetScale3D(FVector::OneVector);
			
			FTransform MantleTargetPlus = DexMathUtility::TransformPlusTransform(MantleTarget, C);
			
			FTransform D = DexMathUtility::TransformLerp(MantleTargetPlus, MantleTarget, PositionAlpha);
			
			FTransform MantleTargetPlusActual = DexMathUtility::TransformPlusTransform(MantleTarget, MantleActualStartOffset);
			
			LerpedTarget = DexMathUtility::TransformLerp(MantleTargetPlusActual, D, DeltaTime);
		}	
		
		FHitResult Out;
		SetActorLocationAndRotationUpdateTarget(OwningCharacter, LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator(), false, false, &Out);
	}	
}

const bool UDexCharacterMovementData::SetActorLocationAndRotationUpdateTarget(ACharacter* OwningCharacter, FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult* OutSweepHitResult)
{
	TargetRotation = NewRotation;
	return OwningCharacter->SetActorLocationAndRotation(NewLocation, TargetRotation, bSweep, OutSweepHitResult, bTeleport ? ETeleportType::TeleportPhysics : ETeleportType::None);
}

void UDexCharacterMovementData::RagdollUpdate(ACharacter* OwningCharacter, const float DeltaTime)
{
	if (USkeletalMeshComponent* MeshComp = OwningCharacter->GetMesh())
	{
		LastRagdollVelocity = MeshComp->GetPhysicsLinearVelocity(FName(TEXT("root")));
		const float Spring = DexMathUtility::RangeClamped(LastRagdollVelocity.Size(), 0.f, 1000.f, 0.f, 25000.f);
		MeshComp->SetAllMotorsAngularDriveParams(Spring, 0.f, 0.f, false);
		MeshComp->SetEnableGravity(LastRagdollVelocity.Z > -4000.f);
		SetActorLocationDuringRagdoll(OwningCharacter);
	}
}

void UDexCharacterMovementData::SetActorLocationDuringRagdoll(ACharacter* OwningCharacter)
{
	FVector TargetRagdollLocation = FVector::ZeroVector;
	FRotator TargetRagdollRotation = FRotator::ZeroRotator;
	if (USkeletalMeshComponent* MeshComp = OwningCharacter->GetMesh())
	{
		TargetRagdollLocation = MeshComp->GetSocketLocation(FName(TEXT("pelvis")));
		const FRotator PelvisRotation = MeshComp->GetSocketRotation(FName(TEXT("pelvis")));
		bRagdollFaceUp = PelvisRotation.Roll < 0.f;
		TargetRagdollRotation = bRagdollFaceUp ? FRotator(0.f, PelvisRotation.Yaw - 180.f, 0.f) : FRotator(0.f, PelvisRotation.Yaw, 0.f);
		if (UCapsuleComponent* CapsuleComp = OwningCharacter->GetCapsuleComponent())
		{
			if (UWorld* World = OwningCharacter->GetWorld())
			{
				FVector End = FVector(TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z - CapsuleComp->GetScaledCapsuleHalfHeight());
				FHitResult LineHit;
				if (World->LineTraceSingleByChannel(LineHit, TargetRagdollLocation, End, ECollisionChannel::ECC_Visibility))
				{
					bRagdollOnGround = LineHit.bBlockingHit;
					if (bRagdollOnGround)
					{
						FVector NewLocation = FVector(TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z + (CapsuleComp->GetScaledCapsuleHalfHeight() - FMath::Abs(LineHit.ImpactPoint.Z - LineHit.TraceStart.Z)) + 2.f);
						FHitResult None;
						SetActorLocationAndRotationUpdateTarget(OwningCharacter, NewLocation, TargetRagdollRotation, false, false, &None);
					}
					else
					{
						FHitResult None;
						SetActorLocationAndRotationUpdateTarget(OwningCharacter, TargetRagdollLocation, TargetRagdollRotation, false, false, &None);
					}
				}
			}
		}
	}
}