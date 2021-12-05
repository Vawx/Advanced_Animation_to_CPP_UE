
#include "Character/DexCharacterAnimInst.h"
#include "Character/DexCharacterBase.h"
#include "Utility/DexMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

UDexCharacterAnimInst::UDexCharacterAnimInst(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	bJumping = false;
	Speed = 0.f;
	Velocity = FVector::ZeroVector;

	PreviousMovementState = ECharacterMovementState::Grounded;
	ViewMode = ECharacterViewMode::ThirdPerson;
	CharacterStance = ECharacterStance::Standing;
	RotationMode = ECharacterRotationMode::LookingDirection;

	BasePoseNormal = 1.f;
}

void UDexCharacterAnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	
}

void UDexCharacterAnimInst::NativeUpdateAnimation(float DeltaTime)
{	
	LastDelta = DeltaTime;
	if (ADexCharacterBase* Character = GetCharacter<ADexCharacterBase>())
	{
		bJumping = Character->bJumping;
		Velocity = Character->VelocityXY;
		Speed = Velocity.Size();
		MovementState = Character->MovementState;
		LocomotionState = Character->LocomotionState;
		bHasMovementInput = Character->bHasMovementInput;
		bIsMoving = Character->bIsMoving;
		PreviousMovementState = Character->PreviousMovementState;
		ViewMode = Character->ViewMode;
		CharacterStance = Character->CharacterStance;
		LocomotionState = Character->LocomotionState;
		RotationMode = Character->RotationMode;
		AimingRotation = Character->GetControlRotation();
		AimYawRate = Character->AimYawRate;
		Acceleration = Character->Acceleration;

		if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
		{
			MovementInput = Movement->GetCurrentAcceleration();
		}

		UpdateAiming();
		UpdateLayerValues();
		UpdateFeet();

		if (MovementState == ECharacterMovementState::Grounded)
		{
			static bool LocalMoveState = false;
			bShouldMove = ShouldMove();
			if (LocalMoveState != bShouldMove)
			{
				LocalMoveState = bShouldMove;
				if (LocalMoveState)
				{
					ElapsedDelayTime = 0.f;
					RotateL = false;
					RotateR = false;
				}
			}

			if (bShouldMove)
			{
				LocalMoveState = true;
				UpdateMovementValues();
				UpdateRotationValues();
			}
			else
			{
				const FName EnableTransition = FName(TEXT("Enable_Transition"));
				const float EnableTransitionValue = GetCurveValue(EnableTransition);

				LocalMoveState = false;

				// Rotate in place
				if (RotationMode == ECharacterRotationMode::AimingDirection ||
					ViewMode == ECharacterViewMode::FirstPerson)
				{
					const float RotateMinThreshold = -50.f;
					const float RotateMaxThreshold = 50.f;
					bRotateLeft = AimingAngle.X < RotateMinThreshold;
					bRotateRight = AimingAngle.X > RotateMaxThreshold;

					if (bRotateLeft || bRotateRight)
					{
						const float AimYawRateMinRange = 90.f;
						const float AimYawRateMaxRange = 270.f;
						const float MinPlayRate = 1.15f;
						const float MaxPlayRate = 3.f;

						RotateRate = DexMathUtility::RangeClamped(AimYawRate, AimYawRateMinRange, AimYawRateMaxRange, MinPlayRate, MaxPlayRate);
					}
				}
				else
				{
					bRotateLeft = false;
					bRotateRight = false;
				}

				// Turn in place
				if (RotationMode == ECharacterRotationMode::LookingDirection &&
					ViewMode == ECharacterViewMode::ThirdPerson &&
					EnableTransitionValue > 0.99)
				{
					const float TurnCheckMinAngle = 45.f;
					const float AimYawRateLimit = 50.f;

					float ABSAngleX = FMath::Abs(AimingAngle.X);
					if (ABSAngleX > TurnCheckMinAngle && AimYawRate < AimYawRateLimit)
					{
						ElapsedDelayTime += LastDelta;
						const float MinAngleDelay = 0.75f;
						const float MaxAngleDelay = 0.f;

						float AngleMapped = DexMathUtility::RangeClamped(ABSAngleX, TurnCheckMinAngle, 180.f, MinAngleDelay, MaxAngleDelay);
						if (ElapsedDelayTime > AngleMapped)
						{
							FRotator Target = FRotator(0.f, AimingRotation.Yaw, 0.f);
							FRotator TargetRotDelta = DexMathUtility::NormalizedRotators(Target, Character->GetActorRotation());
							TurnAngle = TargetRotDelta.Yaw;

							const float Turn180Threshold = 130.f;
							float TurnAngleAbs = FMath::Abs(TurnAngle);
							if (TurnAngleAbs < Turn180Threshold)
							{
								if (TurnAngle < 0.f)
								{
									TurnInPlaceAsset = TurnInPlaceLeft90;
								}
								else
								{
									TurnInPlaceAsset = TurnInPlaceRight90;
								}
							}
							else
							{
								if (TurnAngle < 0.f)
								{
									TurnInPlaceAsset = TurnInPlaceLeft180;									
								}
								else
								{
									TurnInPlaceAsset = TurnInPlaceLeft180;									
								}
							}

							if (false == IsPlayingSlotAnimation(TurnInPlaceAsset.Animation, TurnInPlaceAsset.SlotName))
							{
								PlaySlotAnimationAsDynamicMontage(TurnInPlaceAsset.Animation, TurnInPlaceAsset.SlotName, 0.2f, 0.2f, TurnInPlaceAsset.PlayRate, 1, 0.f, 0.f);

								if (TurnInPlaceAsset.bScaleTurnAngle)
								{
									float TurnAngleDivisor = TurnAngle / TurnInPlaceAsset.AnimationAngle;
									RotationScale = TurnAngleDivisor * TurnInPlaceAsset.PlayRate;
								}
								else
								{
									RotationScale = TurnInPlaceAsset.PlayRate;
								}
							}
						}
					}
					else
					{
						ElapsedDelayTime = 0.f;
					}
				}
				else
				{
					ElapsedDelayTime = 0.f;
				}

				// Dynamic transitions
				if (FMath::IsNearlyEqual(EnableTransitionValue, 1.f))
				{
				}
			}
		}
		else if (MovementState == ECharacterMovementState::InAir)
		{
			FallSpeed = FMath::Abs(Character->Velocity.Z);		
			
			// Landing prediction
			if (FallSpeed < -200.f && LandPredictionCurve != nullptr)
			{
				if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
				{
					if (UWorld* World = GetWorld())
					{
						FVector Start = Capsule->GetComponentLocation();
						float Z = FMath::Clamp(Velocity.Z, -4000.f, -200.f);
						float ZLength = DexMathUtility::RangeClamped(Velocity.Z, 0.f, -4000.f, 50.f, 2000.f);
						FVector End = FVector(Velocity.X, Velocity.Y, Z);
						End *= ZLength;
						End += Start;						
						FHitResult CapsuleTrace;
						if (World->SweepSingleByProfile(CapsuleTrace, Start, End, FQuat::Identity, FName(TEXT("Character")), FCollisionShape::MakeCapsule(Capsule->GetUnscaledCapsuleRadius(), Capsule->GetUnscaledCapsuleHalfHeight())))
						{
							if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
							{
								if (Movement->IsWalkable(CapsuleTrace) && CapsuleTrace.bBlockingHit)
								{
									float PredictionCurveValue = LandPredictionCurve->GetFloatValue(CapsuleTrace.Time);
									LandPrediction = FMath::Lerp(PredictionCurveValue, 0.f, GetCurveValue(FName(TEXT("Mask_LandPrediction"))));
								}
								else
								{
									LandPrediction = 0.f;
								}
							}
						}
					}
				}
			}
			else
			{
				LandPrediction = 0.f;
			}

			// In air lean
			if (LeanInAirCurve)
			{
				FRotator CharRotation = Character->GetActorRotation();
				FVector VelUnrotated = CharRotation.UnrotateVector(Velocity);
				FVector VelUnrotatedDivisor = VelUnrotated / 350.f;
				FVector2D A = FVector2D(VelUnrotatedDivisor.Y, VelUnrotatedDivisor.X);
				float LeanInAirCurveValue = LeanInAirCurve->GetFloatValue(FallSpeed);
				FVector2D B = A * LeanInAirCurveValue;
				LeanAmount.LR = B.X;
				LeanAmount.FB = B.Y;
			}
		}
	}

	Super::NativeUpdateAnimation(DeltaTime);
}

void UDexCharacterAnimInst::Jumped()
{
	JumpPlayRate = DexMathUtility::RangeClamped(Speed, 0.f, 600.f, 1.2f, 1.5f);
}

void UDexCharacterAnimInst::UpdateMovementValues()
{
	const float VelocityBlendInterpSpeed = 12.f;
	VelocityBlend = InterpVelocityBlend(VelocityBlend, CalculateVelocityBlend(), VelocityBlendInterpSpeed);
	if (UCurveFloat* Curve = DiagonalScaleAmountCurve)
	{
		DiagonalScaleAmount = Curve->GetFloatValue(FMath::Abs(VelocityBlend.F + VelocityBlend.B));
	}

	RelativeAccelerationAmount = FVector::ZeroVector;
	if (ADexCharacterBase* Character = GetCharacter<ADexCharacterBase>())
	{
		if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
		{
			FVector A = FVector::ZeroVector;
			FVector B = FVector::ZeroVector;

			FRotator CharacterRotation = Character->GetActorRotation();
			float MaxAccel = Movement->GetMaxAcceleration();
			float MaxBrake = Movement->GetMaxBrakingDeceleration();
			if (FVector::DotProduct(Acceleration, Velocity) > 0.f)
			{
				A = DexMathUtility::VectorClampSizeMax(Acceleration, MaxAccel);
				B = A / MaxAccel;
			}
			else
			{
				A = DexMathUtility::VectorClampSizeMax(Acceleration, MaxBrake);
				B = A / MaxBrake;
			}

			RelativeAccelerationAmount = CharacterRotation.UnrotateVector(B);
		}
	}

	const float GroundedLeanInterpSpeed = 4.f;
	FLeanAmount Target = {};
	Target.LR = RelativeAccelerationAmount.Y;
	Target.FB = RelativeAccelerationAmount.X;

	LeanAmount.LR = FMath::FInterpTo(LeanAmount.LR, Target.LR, LastDelta, GroundedLeanInterpSpeed);
	LeanAmount.FB = FMath::FInterpTo(LeanAmount.FB, Target.FB, LastDelta, GroundedLeanInterpSpeed);

	WalkRunBlend = (LocomotionState == ECharacterLocomotionState::Walking) ? 0.f : 1.f;

	const FName WeightGait = FName(TEXT("Weight_Gait"));
	const FName BasePoseCLF = FName(TEXT("BasePose_CLF"));

	StrideBlend = 0.f;
	if (StrideBlendWalk && StrideBlendRun && StrideBlendCrouchWalk)
	{
		float A = StrideBlendWalk->GetFloatValue(Speed);
		float B = StrideBlendRun->GetFloatValue(Speed);
		float Alpha = DexMathUtility::AnimCurveClamped(this, WeightGait, -1.f, 0.f, 1.f);
		float C = FMath::Lerp(A, B, Alpha);

		float D = StrideBlendCrouchWalk->GetFloatValue(Speed);
		StrideBlend = FMath::Lerp(C, D, GetCurveValue(BasePoseCLF));
	}
	
	if (USkeletalMeshComponent* OwningMesh = GetOwningComponent())
	{
		FVector OwningMeshScale = OwningMesh->GetComponentScale();
		{
			const float AnimatedWalkSpeed = 150.f;
			const float AnimatedRunSpeed = 350.f;
			const float AnimatedSprintSpeed = 600.f;
			float A = Speed / AnimatedWalkSpeed;
			float B = Speed / AnimatedRunSpeed;
			float Alpha = DexMathUtility::AnimCurveClamped(this, WeightGait, -1.f, 0.f, 1.f);
			float ALerp = FMath::Lerp(A, B, Alpha);
			float C = Speed / AnimatedSprintSpeed;
			float BAlpha = DexMathUtility::AnimCurveClamped(this, WeightGait, -2.f, 0.f, 1.f);
			float BLerp = FMath::Lerp(ALerp, C, BAlpha);
			float D = BLerp / StrideBlend;
			float E = OwningMeshScale.Z;
			float F = D / E;
			StandingPlayRate = FMath::Clamp(F, 0.f, 3.f);
		}
		
		const float AnimatedCrouchSpeed = 150.f;
		float A = Speed / AnimatedCrouchSpeed;
		float B = A / StrideBlend;
		float C = B / OwningMeshScale.Z;
		CrouchingPlayRate = FMath::Clamp(C, 0.f, 2.f);
	}
}

void UDexCharacterAnimInst::UpdateRotationValues()
{
	MovementDirection = GetMovementDirection();

	if (ADexCharacterBase* Character = GetCharacter<ADexCharacterBase>())
	{
		FRotator ControlRot = Character->GetControlRotation();
		FRotator VelRot = Velocity.Rotation();
		if (YawOffsetFrontBack && YawOffsetLeftRight)
		{
			FRotator Delta = DexMathUtility::NormalizedRotators(VelRot, ControlRot);
			float Time = Delta.Yaw;

			FVector FB = YawOffsetFrontBack->GetVectorValue(Time);
			FVector LR = YawOffsetLeftRight->GetVectorValue(Time);

			FYaw = FB.X;
			BYaw = FB.Y;
			LYaw = LR.X;
			RYaw = LR.Y;
		}
	}
}

FVelocityBlend UDexCharacterAnimInst::InterpVelocityBlend(const FVelocityBlend& Current, const FVelocityBlend& Target, const float InterpSpeed)
{
	FVelocityBlend Result = {};
	Result.F = FMath::FInterpTo(Current.F, Target.F, LastDelta, InterpSpeed);
	Result.B = FMath::FInterpTo(Current.B, Target.B, LastDelta, InterpSpeed);
	Result.L = FMath::FInterpTo(Current.L, Target.L, LastDelta, InterpSpeed);
	Result.R = FMath::FInterpTo(Current.R, Target.R, LastDelta, InterpSpeed);	
	return Result;
}

FVelocityBlend UDexCharacterAnimInst::CalculateVelocityBlend()
{
	FVelocityBlend Result = {};
	if (ADexCharacterBase* Character = GetCharacter<ADexCharacterBase>())
	{
		FVector VelDir = Velocity.GetSafeNormal();
		FRotator ActorRot = Character->GetActorRotation();
		FVector LocalRelativeVelocityDirector = ActorRot.UnrotateVector(VelDir);

		float Sum = FMath::Abs(LocalRelativeVelocityDirector.X) + FMath::Abs(LocalRelativeVelocityDirector.Y) + FMath::Abs(LocalRelativeVelocityDirector.Z);

		FVector RelativeDirection = LocalRelativeVelocityDirector / Sum;

		Result.F = FMath::Clamp(RelativeDirection.X, 0.f, 1.f);
		Result.B = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.f, 0.f));
		Result.L = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.f, 0.f));
		Result.R = FMath::Clamp(RelativeDirection.Y, 0.f, 1.f);		
	}
	return Result;
}

ECharacterMovementDirection UDexCharacterAnimInst::GetMovementDirection()
{
	if (LocomotionState == ECharacterLocomotionState::Sprinting)
	{
		return ECharacterMovementDirection::Forward;
	}

	if (RotationMode == ECharacterRotationMode::VelocityDirection)
	{
		return ECharacterMovementDirection::Forward;
	}

	FRotator VelRot = Velocity.Rotation();
	FRotator Delta = DexMathUtility::NormalizedRotators(VelRot, AimingRotation);
	float Angle = Delta.Yaw;
	return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f, Angle);
}

ECharacterMovementDirection UDexCharacterAnimInst::CalculateQuadrant(ECharacterMovementDirection Dir, const float FRThreshold, const float FLThreshold, const float BRThreshold, const float BLThreshold, const float Buffer, const float Angle)
{
	bool A = AngleInRange(Angle, FLThreshold, FRThreshold, Buffer, Dir != ECharacterMovementDirection::Forward || Dir != ECharacterMovementDirection::Backward);
	bool B = AngleInRange(Angle, FRThreshold, BRThreshold, Buffer, Dir != ECharacterMovementDirection::Right || Dir != ECharacterMovementDirection::Left);
	bool C = AngleInRange(Angle, BLThreshold, FLThreshold, Buffer, Dir != ECharacterMovementDirection::Right || Dir != ECharacterMovementDirection::Left);

	if (A) return ECharacterMovementDirection::Forward;
	if (B) return ECharacterMovementDirection::Right;
	if (C) return ECharacterMovementDirection::Left;
	return ECharacterMovementDirection::Backward;	
}

bool UDexCharacterAnimInst::AngleInRange(const float Angle, const float Min, const float Max, const float Buffer, const bool bIncreaseBuffer)
{
	if (bIncreaseBuffer)
	{
		float A = Min - Buffer;
		float B = Max + Buffer;
		return DexMathUtility::InRangeFloatFloat(Angle, A, B);
	}

	float A = Min + Buffer;
	float B = Max - Buffer;
	return DexMathUtility::InRangeFloatFloat(Angle, A, B);
}


bool UDexCharacterAnimInst::PlayTransitionAnimation(FDynamicMontageParams Params)
{
	UAnimMontage* Result = PlaySlotAnimationAsDynamicMontage(Params.Animation, Params.SlotName, Params.BlendInTime, Params.BlendOutTime, Params.PlayRate, 1, -1.f, Params.StartTime);
	return Result != nullptr;
}	

void UDexCharacterAnimInst::SetHipsDirection(ECharacterHipsDirection Dir)
{
	HipsDirection = Dir;
}

bool UDexCharacterAnimInst::ShouldMove()
{
	return (bIsMoving && bHasMovementInput) || Speed > 150.f;
}

void UDexCharacterAnimInst::UpdateAiming()
{
	const float SmoothedAimingRotationInterpSpeed = 10.f;
	SmoothedAimingRotation = FMath::RInterpTo(SmoothedAimingRotation, AimingRotation, LastDelta, SmoothedAimingRotationInterpSpeed);

	if (ACharacter* Character = GetCharacter<ACharacter>())
	{
		FRotator CharacterRotation = Character->GetActorRotation();
		FRotator AimingDeltaRotation = DexMathUtility::NormalizedRotators(AimingRotation, CharacterRotation);
		AimingAngle.X = AimingDeltaRotation.Yaw;
		AimingAngle.Y = AimingDeltaRotation.Pitch;
		
		FRotator SmoothedAimingDeltaRotation = DexMathUtility::NormalizedRotators(SmoothedAimingRotation, CharacterRotation);
		SmoothedAimingAngle.X = SmoothedAimingDeltaRotation.Yaw;
		SmoothedAimingAngle.Y = SmoothedAimingDeltaRotation.Pitch;

		if (RotationMode != ECharacterRotationMode::VelocityDirection)
		{
			AimSweepTime = DexMathUtility::RangeClamped(AimingAngle.Y, -90.f, 90.f, 1.f, 0.f);
			SpineRotaion = FRotator::ZeroRotator;
			SpineRotaion.Yaw = (AimingAngle.X / 4.f);
		}
		else
		{
			if (bHasMovementInput)
			{
				const float InputYawOffsetInterpSpeed = 8.f;

				FRotator MovementDeltaRotation = DexMathUtility::NormalizedRotators(MovementInput.Rotation(), CharacterRotation);
				float MappedDelta = DexMathUtility::RangeClamped(MovementDeltaRotation.Yaw, -180.f, 180.f, 0.f, 1.f);
				InputYawOffsetTime = FMath::FInterpTo(InputYawOffsetTime, MappedDelta, LastDelta, InputYawOffsetInterpSpeed);
			}
		}		
	}

	float SmoothedAngleX = SmoothedAimingAngle.X;
	float SmoothedAngleXAbs = FMath::Abs(SmoothedAngleX);

	LeftYawTime = DexMathUtility::RangeClamped(SmoothedAngleXAbs, 0.f, 180.f, 0.5f, 0.f);
	RightYawTime = DexMathUtility::RangeClamped(SmoothedAngleXAbs, 0.f, 180.f, 0.5f, 1.f);
	ForwardYawTime = DexMathUtility::RangeClamped(SmoothedAngleX, -180.f, 180.f, 0.f, 1.f);
}

void UDexCharacterAnimInst::UpdateLayerValues()
{
	const FName Mask_AimOffset = FName(TEXT("Mask_AimOffset"));
	const FName BasePose_N = FName(TEXT("BasePose_N"));
	const FName BasePose_CLF = FName(TEXT("BasePose_CLF"));
	const FName Layering_Spine_Add = FName(TEXT("Layering_Spine_Add"));
	const FName Layering_Head_Add = FName(TEXT("Layering_Head_Add"));
	const FName Layering_Arm_L_Add = FName(TEXT("Layering_Arm_L_Add"));
	const FName Layering_Arm_R_Add = FName(TEXT("Layering_Arm_R_Add"));
	const FName Layering_Hand_R = FName(TEXT("Layering_Hand_R"));
	const FName Layering_Hand_L = FName(TEXT("Layering_Hand_L"));
	const FName Enable_HandIK_L = FName(TEXT("Enable_HandIK_L"));
	const FName Layering_Arm_L = FName(TEXT("Layering_Arm_L"));
	const FName Enable_HandIK_R = FName(TEXT("Enable_HandIK_R"));
	const FName Layering_Arm_R = FName(TEXT("Layering_Arm_R"));
	const FName Layering_Arm_L_LS = FName(TEXT("Layering_Arm_L_LS"));
	const FName Layering_Arm_R_LS = FName(TEXT("Layering_Arm_R_LS"));

	float AimOffsetValue = GetCurveValue(Mask_AimOffset);
	EnableAimOffset = FMath::Lerp(1.f, 0.f, AimOffsetValue);
	BasePoseNormal = GetCurveValue(BasePose_N);
	BasePoseCrouched = GetCurveValue(BasePose_CLF);
	SpineAdd = GetCurveValue(Layering_Spine_Add);
	HeadAdd = GetCurveValue(Layering_Head_Add);
	ArmLAdd = GetCurveValue(Layering_Arm_L_Add);
	ArmRAdd = GetCurveValue(Layering_Arm_R_Add);
	HandR = GetCurveValue(Layering_Hand_R);
	HandL = GetCurveValue(Layering_Hand_L);
	EnableHandIKL = GetCurveValue(Enable_HandIK_L);
	EnableHandIKR = GetCurveValue(Enable_HandIK_R);
	ArmLLS = GetCurveValue(Layering_Arm_L_LS);
	ArmLMS = 1.f - FMath::Floor(ArmLLS);
	ArmRLS = GetCurveValue(Layering_Arm_R_LS);
	ArmRMS = 1.f - FMath::Floor(ArmRLS);
}

void UDexCharacterAnimInst::UpdateFeet()
{
	USkeletalMeshComponent* OwningComp = GetOwningComponent();
	if (nullptr == OwningComp)
	{
		return;
	}

	auto Foot = [&](const FName Enable, const FName Lock, const FName Bone, FVector& Location, FRotator& Rotation, float& Alpha) {
		ACharacter* Character = GetCharacter<ACharacter>();
		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
		if (nullptr == Character || nullptr == Movement)
		{
			return;
		}
	
		float FootIKValue = GetCurveValue(Enable);
		if (FootIKValue > 0.f)
		{
			float FootLockCurve = GetCurveValue(Lock);
			if (FootLockCurve >= 0.99f || FootLockCurve < Alpha)
			{
				Alpha = FootLockCurve;				
			}		
			
			if (Alpha >= 0.99f)
			{
				FTransform IKTransform = OwningComp->GetSocketTransform(Bone, ERelativeTransformSpace::RTS_Component);
				Location = IKTransform.GetLocation();
				Rotation = IKTransform.GetRotation().Rotator();						
			}

			if (Alpha > 0.f)
			{
				FRotator RotationDifference = FRotator::ZeroRotator;
				FVector LocationDifference = FVector::ZeroVector;				
				
				if (Movement->IsMovingOnGround())
				{
					FRotator Delta = DexMathUtility::NormalizedRotators(Character->GetActorRotation(), Movement->GetLastUpdateRotation());
					RotationDifference = Delta;
				}					
				
				FVector VelOverDelta = Velocity * LastDelta;
				FRotator OwningRot = OwningComp->GetComponentRotation();
				LocationDifference = OwningRot.UnrotateVector(VelOverDelta);
				
				FVector Diff = Location - LocationDifference;
				FVector Axis = Diff.RotateAngleAxis(RotationDifference.Yaw, FVector(0.f, 0.f, -1.f));
				Location = Axis;
				
				FRotator Delta = DexMathUtility::NormalizedRotators(Rotation, RotationDifference);
				Rotation = Delta;
			}
		}
	};

	const FName Enable_FootIK_L = FName(TEXT("Enable_FootIK_L"));
	const FName Enable_FootIK_R = FName(TEXT("Enable_FootIK_R"));	
	const FName FootLock_L = FName(TEXT("FootLock_L"));	
	const FName FootLock_R = FName(TEXT("FootLock_R"));
	const FName ik_foot_l = FName(TEXT("ik_foot_l"));
	const FName ik_foot_r = FName(TEXT("ik_foot_r"));
	const FName root = FName(TEXT("root"));

	Foot(Enable_FootIK_L, FootLock_L, ik_foot_l, FootLockLLocation, FootLockLRotation, FootLockLAlpha);
	Foot(Enable_FootIK_R, FootLock_R, ik_foot_r, FootLockRLocation, FootLockRRotation, FootLockRAlpha);

#if DEXCHARACTER_ANIMINST_DEBUG_LOCK_OFFSET
	if (UWorld* World = GetWorld())
	{
		FTransform Left = OwningComp->GetSocketTransform(FName(TEXT("ik_foot_l")), ERelativeTransformSpace::RTS_World);
		FTransform Right = OwningComp->GetSocketTransform(FName(TEXT("ik_foot_r")), ERelativeTransformSpace::RTS_World);
		
		DrawDebugBox(World, Left.GetLocation() + FootLockLLocation, FVector(12.f, 12.f, 12.f), FQuat::Identity, FColor::Yellow, false, LastDelta * 3.f);
		DrawDebugBox(World, Right.GetLocation() + FootLockRLocation, FVector(12.f, 12.f, 12.f), FQuat::Identity, FColor::Yellow, false, LastDelta * 3.f);		
	}
#endif

	auto SetPelvisOffset = [&](const FVector OffsetLTarget, const FVector OffsetRTarget) {
		float IKL = GetCurveValue(Enable_FootIK_L);
		float IKR = GetCurveValue(Enable_FootIK_R);
		float IKCombine = IKL + IKR;
		PelvisAlpha = IKCombine / 2.f;

		FVector PelvisTarget = FVector::ZeroVector;
		if (PelvisAlpha > 0.f)
		{
			if (OffsetLTarget.Z < OffsetRTarget.Z)
			{
				PelvisTarget = OffsetLTarget;
			}
			else
			{
				PelvisTarget = OffsetRTarget;				
			}

			if (PelvisTarget.Z > PelvisOffset.Z)
			{
				PelvisOffset = FMath::VInterpTo(PelvisOffset, PelvisTarget, LastDelta, 10.f);
			}
			else
			{
				PelvisOffset = FMath::VInterpTo(PelvisOffset, PelvisTarget, LastDelta, 15.f);				
			}
		}
		else
		{
			PelvisOffset = FVector::ZeroVector;
		}
	};

	if (MovementState == ECharacterMovementState::None || MovementState == ECharacterMovementState::Grounded)
	{
		auto DoOffset = [&](const FName Enable, const FName IKBone, const FName RootBone, FVector& Target, FVector& Offset, FRotator& Rotation)
		{
			float EnableValue = GetCurveValue(Enable);
			if (EnableValue > 0.f)
			{
				if (ACharacter* Character = GetCharacter<ACharacter>())
				{
					if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
					{
						FVector IkBoneLocation = OwningComp->GetSocketLocation(IKBone);
						FVector RootBoneLocation = OwningComp->GetSocketLocation(RootBone);
						FVector IKRoot = FVector(IkBoneLocation.X, IkBoneLocation.Y, RootBoneLocation.Z);

						const float IKTraceDistanceUp = 50.f;
						const float IKTraceDistanceDown = 45.f;
						const float FootHeight = 13.5f;

						FVector Start = IKRoot + FVector(0.f, 0.f, IKTraceDistanceUp);
						FVector End = IKRoot - FVector(0.f, 0.f, IKTraceDistanceDown);

						FRotator TargetRotation = FRotator::ZeroRotator;
						if (UWorld* World = GetWorld())
						{
							FHitResult FootLineTrace;
							FCollisionQueryParams Params;
							Params.AddIgnoredActor(Character);
							if (World->LineTraceSingleByChannel(FootLineTrace, Start, End, ECollisionChannel::ECC_Visibility, Params))
							{
								if (Movement->IsWalkable(FootLineTrace))
								{
									FVector HitLocation = FootLineTrace.ImpactPoint;
									FVector HitNormal = FootLineTrace.ImpactNormal;

									FVector A = HitNormal * FootHeight;
									FVector B = HitLocation + A;
									FVector C = IKRoot + (FVector(0.f, 0.f, 1.f) * FootHeight);
									Target = B - C;

									float AtanA = FMath::Atan2(HitNormal.Y, HitNormal.Z);
									float AtanB = FMath::Atan2(HitNormal.X, HitNormal.Z) * -1.f;
									TargetRotation = FRotator(AtanB, 0.f, AtanA);
								}
							}
						}
						if (Offset.Z > Target.Z)
						{
							Offset = FMath::VInterpTo(Offset, Target, LastDelta, 30.f);
						}
						else
						{
							Offset = FMath::VInterpTo(Offset, Target, LastDelta, 15.f);
						}
						
						Rotation = FMath::RInterpTo(Rotation, TargetRotation, LastDelta, 30.f);
					}
				}
			}
			else
			{
				Target = FVector::ZeroVector;
				Rotation = FRotator::ZeroRotator;
			}
		};

		DoOffset(Enable_FootIK_L, ik_foot_l, root, FootOffsetLTarget, FootOffsetLLocation, FootOffsetLRotation);
		DoOffset(Enable_FootIK_R, ik_foot_r, root, FootOffsetRTarget, FootOffsetRLocation, FootOffsetRRotation);	
		
#if DEXCHARACTER_ANIMINST_DEBUG_LOCK_OFFSET
		if (UWorld* World = GetWorld())
		{
			FVector Left = OwningComp->GetSocketLocation(FName(TEXT("VB ik_foot_l_Offset")));
			FVector Right = OwningComp->GetSocketLocation(FName(TEXT("VB ik_foot_r_Offset")));
			DrawDebugSphere(World, Left + FootOffsetLLocation, 12.f, 12, FColor::Red, false, LastDelta * 3.f);
			DrawDebugSphere(World, Right + FootOffsetRLocation, 12.f, 12, FColor::Red, false, LastDelta * 3.f);			
		}
#endif

		SetPelvisOffset(FootOffsetLTarget, FootOffsetRTarget);

#if DEXCHARACTER_ANIMINST_DEBUG_LOCK_OFFSET
		if (UWorld* World = GetWorld())
		{
			FVector Start = OwningComp->GetSocketLocation(FName(TEXT("Pelvis")));
			DrawDebugSphere(World, Start + PelvisOffset, 16.f, 12.f, FColor::Green, false, LastDelta * 3.f);
		}
#endif
	}
	else
	{
		SetPelvisOffset(FVector::ZeroVector, FVector::ZeroVector);
		FootOffsetLLocation = FMath::VInterpTo(FootOffsetLLocation, FVector::ZeroVector, LastDelta, 15.f);
		FootOffsetRLocation = FMath::VInterpTo(FootOffsetRLocation, FVector::ZeroVector, LastDelta, 15.f);

		FootOffsetLRotation = FMath::RInterpTo(FootOffsetLRotation, FRotator::ZeroRotator, LastDelta, 15.f);
		FootOffsetRRotation = FMath::RInterpTo(FootOffsetRRotation, FRotator::ZeroRotator, LastDelta, 15.f);		
	}

#if DEXCHARACTER_ANIMINST_DEBUG_IK
	if (UWorld* World = GetWorld())
	{
		FVector ik_foot_r_offset = OwningComp->GetSocketLocation(FName(TEXT("VB ik_foot_r_offset")));
		FVector ik_foot_l_offset = OwningComp->GetSocketLocation(FName(TEXT("VB ik_foot_l_offset")));
		DrawDebugSphere(World, ik_foot_r_offset + FootOffsetRLocation, 8.f, 12, FColor::Yellow, false, 0.03f);
		DrawDebugSphere(World, ik_foot_l_offset + FootOffsetLLocation, 8.f, 12, FColor::Yellow, false, 0.03f);
		
		FVector ik_knee_target_l = OwningComp->GetSocketLocation(FName(TEXT("VB ik_knee_target_l")));
		FVector ik_knee_target_r = OwningComp->GetSocketLocation(FName(TEXT("VB ik_knee_target_r")));
		DrawDebugSphere(World, ik_knee_target_l, 8.f, 12, FColor::Green, false, 0.03f);
		DrawDebugSphere(World, ik_knee_target_r, 8.f, 12, FColor::Green, false, 0.03f);

		DrawDebugLine(World, ik_knee_target_l, ik_foot_l_offset, FColor::Red, false, 0.03f);
		DrawDebugLine(World, ik_knee_target_r, ik_foot_r_offset, FColor::Red, false, 0.03f);		
	}
#endif
}


void UDexCharacterAnimInst::SetFootLocation(bool bLeft, FVector Location, FRotator Rotation, float Alpha)
{
	if (bLeft)
	{
		FootLockLLocation = Location;
		FootLockLRotation = Rotation;
		FootLockLAlpha = Alpha;
	}
	else
	{
		FootLockRLocation = Location;
		FootLockRRotation = Rotation;
		FootLockRAlpha = Alpha;
	}
}

void UDexCharacterAnimInst::SetFootOffset(bool bLeft, FVector Location, FRotator Rotation)
{
	if (bLeft)
	{
		FootOffsetLLocation = Location;
		FootOffsetLRotation = Rotation;
	}
	else
	{
		FootOffsetRLocation = Location;
		FootOffsetRRotation = Rotation;
	}
}