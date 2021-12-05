#include "Character/DexCharacterBase.h"
#include "Character/DexPlayerStateBase.h"
#include "Utility/DexMathUtility.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Character/DexCharacterAnimInst.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogDexCharacter);

ADexCharacterBase::ADexCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;	

	MovementState = ECharacterMovementState::Grounded;
	PreviousMovementState = ECharacterMovementState::None;
	LocomotionState = ECharacterLocomotionState::Walking;
	DesiredLocomotionState = ECharacterLocomotionState::Running;
	ViewMode = ECharacterViewMode::ThirdPerson;
	CharacterStance = ECharacterStance::Standing;
	RotationMode = ECharacterRotationMode::LookingDirection;
	DesiredRotationMode = ECharacterRotationMode::LookingDirection;
	MovementAction = ECharacterMovementAction::None;
}

void ADexCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		MeshComponent->AddTickPrerequisiteActor(this);
	}
	
	ensure(SetMovementModel());
	ChangeRotationMode(DesiredRotationMode);
	ChangeLocomotionState(DesiredLocomotionState);
	ChangeViewMode(ViewMode);

	FRotator ActorRot = GetActorRotation();
	TargetRotation = ActorRot;
	LastVelocityRotation = ActorRot;
	LastMovementInputRotation = ActorRot;

	if (ensure(AbilitySystemComponent) && DefaultAbilitySet)
	{
		DefaultAbilitySet->GiveAbilities(AbilitySystemComponent);
	}
}

void ADexCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastDeltaTime = DeltaTime;

	SetFrameTimeData();

	if (MovementState == ECharacterMovementState::Grounded)
	{
		UpdateCharacterMovment();
		UpdateGroundRotation();
	}
	else
	{
		// Air
		// Mantling
		// Ragdoll
	}
}

void ADexCharacterBase::SetupPlayerInputComponent(UInputComponent* PIC)
{
	if (ensure(PIC))
	{
		PIC->BindAxis(TEXT("Forward"), this, &ADexCharacterBase::Forward);
		PIC->BindAxis(TEXT("Right"), this, &ADexCharacterBase::Right);
		PIC->BindAxis(TEXT("Look"), this, &ADexCharacterBase::Look);
		PIC->BindAxis(TEXT("Up"), this, &ADexCharacterBase::Up);

		PIC->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);		
		PIC->BindAction(TEXT("Sprint"), IE_Pressed, this, &ADexCharacterBase::Sprint);
		PIC->BindAction(TEXT("Sprint"), IE_Released, this, &ADexCharacterBase::Sprint);
		PIC->BindAction(TEXT("Aim"), IE_Pressed, this, &ADexCharacterBase::Aim);
		PIC->BindAction(TEXT("Aim"), IE_Released, this, &ADexCharacterBase::Aim);
	}

	if (ensure(AbilitySystemComponent))
	{
		FGameplayAbilityInputBinds AbilityBind(TEXT("AbilityActivate"), TEXT("AbilityCancel"), TEXT("EAbilityInputBinds"), EAbilityInputBinds::KeyOne);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PIC, AbilityBind);
	} 

	Super::SetupPlayerInputComponent(PIC);
}

bool ADexCharacterBase::SetMovementModel()
{
	if (false == MovementModelData.IsNull())
	{
		CharacterMovementStateData = MovementModelData.GetRow<FCharacterMovementSettingsState>(TEXT("MovementModelDataTable"));
		return CharacterMovementStateData != nullptr;
	}
	return false;
}

void ADexCharacterBase::SetFrameTimeData()
{
	Velocity = GetVelocity();
	VelocityXY = Velocity;
	VelocityXY.Z = 0.f;
	Acceleration = (Velocity - PreviousVelocity) / LastDeltaTime;
	Speed = VelocityXY.Size();
	bIsMoving = Speed > 1.f;
	if (bIsMoving)
	{
		LastVelocityRotation = Velocity.Rotation();
	}
	bHasMovementInput = MovementInputAmount() > 0.f;
	if (bHasMovementInput)
	{
		if (UCharacterMovementComponent* Movement = GetCharacterMovement())
		{
			LastMovementInputRotation = Movement->GetCurrentAcceleration().Rotation();
		}
	}

	FRotator ControllerRotation = GetControlRotation();
	AimYawRate = FMath::Abs((ControllerRotation.Yaw - PreviousAimYaw) / LastDeltaTime);

	PreviousAimYaw = ControllerRotation.Yaw;
	PreviousVelocity = Velocity;
}

float ADexCharacterBase::GetAnimCurveValue(const FName& Name)
{
	if (false == Name.IsNone())
	{
		if (UAnimInstance* AnimInst = GetAnimInstance<UAnimInstance>())
		{
			return AnimInst->GetCurveValue(Name);
		}
	}
	UE_LOG(LogDexCharacter, Warning, TEXT("Failed to get a valid AnimCurveValue from ADexCharacterBase->Mesh()->AnimInstance. Is the curve name correct?"));
	return false;
}

UAnimMontage* ADexCharacterBase::GetRollAnimation()
{
	switch (OverlayState)
	{
		case ECharacterOverlayState::Default:
		case ECharacterOverlayState::Masculine:
		case ECharacterOverlayState::Feminine:
		{
			return RollDefault;
		} break;
		case ECharacterOverlayState::Inured:
		case ECharacterOverlayState::Bow:
		case ECharacterOverlayState::Torch:
		case ECharacterOverlayState::Barrel:
		{
			return LandRoll_LH;
		} break;
		case ECharacterOverlayState::HandsTied:
		case ECharacterOverlayState::Pistol:
		case ECharacterOverlayState::Pistol_2H:
		{
			return LandRoll_2H;
		} break;
		case ECharacterOverlayState::Rifle:
		case ECharacterOverlayState::Binoculars:
		case ECharacterOverlayState::Box:
		{
			return LandRoll_RH;
		} break;
	}
	return nullptr;
}

UDexAbilitySystemComponent* ADexCharacterBase::GetAbilitySystemComponent_K2()
{
	return GetAbilityComponent<UDexAbilitySystemComponent>();
}

void ADexCharacterBase::InitAbilityComponent()
{
	if (ADexPlayerStateBase* State = GetState<ADexPlayerStateBase>())
	{
		if (State->AbilitySystemComponent == AbilitySystemComponent)
		{
			return;
		}

		AbilitySystemComponent = State->AbilitySystemComponent;
		AbilitySystemComponent->SetAvatarActor(this);
	}
}

void ADexCharacterBase::Forward(const float Amount)
{
	CharacterRuntimeInputData.ForwardAxisAmount = Amount;
	PlayerInputMovement(true);	
}

void ADexCharacterBase::Right(const float Amount)
{
	CharacterRuntimeInputData.RightAxisAmount = Amount;
	PlayerInputMovement(false);
}

void ADexCharacterBase::Look(const float Amount)
{
	CharacterRuntimeInputData.LookAxisAmount = Amount;
	AddControllerYawInput(Amount);
}

void ADexCharacterBase::Up(const float Amount)
{
	CharacterRuntimeInputData.UpAxisAmount = Amount;
	AddControllerPitchInput(Amount);	
}

void ADexCharacterBase::Aim()
{
	static bool bToggle = false;
	bToggle = !bToggle;
	if (bToggle)
	{
		RotationMode = ECharacterRotationMode::AimingDirection;
	}
	else
	{
		RotationMode = DesiredRotationMode;
	}

}

void ADexCharacterBase::Sprint()
{
	static bool bToggled = false;
	bToggled = !bToggled;
	if (bToggled)
	{
		DesiredLocomotionState = ECharacterLocomotionState::Sprinting;
	}
	else
	{
		DesiredLocomotionState = ECharacterLocomotionState::Running;
	}
}

bool ADexCharacterBase::Roll()
{
	if (MovementState == ECharacterMovementState::Grounded)
	{
		if (UDexCharacterAnimInst* AnimInst = GetAnimInstance<UDexCharacterAnimInst>())
		{
			if (UAnimMontage* Montage = GetRollAnimation())
			{
				RollAttemptTime = 0.f;
				JumpInitiatedTime = 0.f;
				float VelocityLength = VelocityXY.Size();
				float PlayRate = DexMathUtility::RangeClamped(VelocityLength, 0.f, 600.f, 0.9f, 1.8f);
				AnimInst->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, 0.f, true);
				return true;
			}
		} 	
	}

	if (MovementState == ECharacterMovementState::InAir)
	{
		if (UWorld* World = GetWorld())
		{
			RollAttemptTime = 0.f;
			JumpInitiatedTime = 0.f;
			RollAttemptTime = World->GetTimeSeconds();
		}
	}
	return false;
}

void ADexCharacterBase::PlayerInputMovement(bool bForward)
{
	if (AController* Cont = GetController())
	{
		float X = 0.f;
		float Y = 0.f;
		FixDiagonalGameplayValues(Y, X);

		const FQuat ControlRot = Cont->GetControlRotation().Quaternion();
		FVector ForwardVector = ControlRot.GetForwardVector();
		FVector RightVector = ControlRot.GetRightVector();
		
		if (bForward)
		{
			AddMovementInput(ForwardVector, Y);
		}
		else
		{
			AddMovementInput(RightVector, X);
		}
	}
}

void ADexCharacterBase::FixDiagonalGameplayValues(float& YOut, float& XOut)
{
	const float A = FMath::Abs(CharacterRuntimeInputData.ForwardAxisAmount);
	const float B = DexMathUtility::RangeClamped(A, 0.f, 0.6f, 1.f, 1.2f);
	YOut = FMath::Clamp(CharacterRuntimeInputData.ForwardAxisAmount * B, -1.f, 1.f);
	
	const float C = FMath::Abs(CharacterRuntimeInputData.RightAxisAmount);
	const float D = DexMathUtility::RangeClamped(C, 0.f, 0.6f, 1.f, 1.2f);
	XOut = FMath::Clamp(CharacterRuntimeInputData.RightAxisAmount * D, -1.f, 1.f);
}

float ADexCharacterBase::MovementInputAmount()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		float AccelLength = Movement->GetCurrentAcceleration().Size();
		float MaxAccel = Movement->GetMaxAcceleration();
		return AccelLength / MaxAccel > 0.f;
	}
	return 0.f;
}

void ADexCharacterBase::UpdateGroundRotation()
{
	if (MovementAction == ECharacterMovementAction::None)
	{
		if (CanUpdateMovingRotation())
		{
			if (RotationMode == ECharacterRotationMode::VelocityDirection)
			{
				SmoothCharacterRotation(FRotator(0.f, LastVelocityRotation.Yaw, 0.f), 800.f, CalculateGroundRotationRate());
			}

			if (RotationMode == ECharacterRotationMode::LookingDirection)
			{
				if (LocomotionState == ECharacterLocomotionState::Sprinting)
				{
					SmoothCharacterRotation(FRotator(0.f, LastVelocityRotation.Yaw, 0.f), 500.f, CalculateGroundRotationRate());
				}
				else
				{
					float ControllerYaw = GetControlRotation().Yaw;
					float YawOffset = GetAnimCurveValue(FName(TEXT("YawOffset")));
					SmoothCharacterRotation(FRotator(0.f, ControllerYaw + YawOffset, 0.f), 500.f, CalculateGroundRotationRate());
				}
			}

			if (RotationMode == ECharacterRotationMode::AimingDirection)
			{
				FRotator ControlRot = GetControlRotation();
				SmoothCharacterRotation(FRotator(0.f, ControlRot.Yaw, 0.f), 1000.f, 20.f);
			}
		}
		else
		{
			auto UpdateGroundRotationFinishUp = [&]() {
				float RotationAmount = GetAnimCurveValue(FName(TEXT("RotationAmount")));
				float DeltaOffsetFrom30FPS = LastDeltaTime / (1.f / 30.f);
				if (FMath::Abs(RotationAmount) > KINDA_SMALL_NUMBER)
				{
					float DeltaYaw = RotationAmount * DeltaOffsetFrom30FPS;
					AddActorWorldRotation(FRotator(0.f, DeltaYaw, 0.f));
				}
			};

			auto FirstPersonAimFallback = [&]() {
				LimitRotation(-100.f, 100.f, 20.f);
				UpdateGroundRotationFinishUp();
			};			

			if (ViewMode == ECharacterViewMode::ThirdPerson)
			{
				if (RotationMode == ECharacterRotationMode::AimingDirection)
				{
					FirstPersonAimFallback();
				}
			}
			else
			{
				FirstPersonAimFallback();
			}

			if (RotationMode == ECharacterRotationMode::VelocityDirection ||
				RotationMode == ECharacterRotationMode::LookingDirection)
			{
				UpdateGroundRotationFinishUp();
			}

			TargetRotation = GetActorRotation();
		}
	}

	if (MovementAction == ECharacterMovementAction::Roll)
	{
		// TODO
	}
}

bool ADexCharacterBase::CanUpdateMovingRotation()
{
	bool SpeedFast = Speed > 150.f;
	bool MovingAndInput = bIsMoving && bHasMovementInput;
	bool RootMotion = HasAnyRootMotion();

	return !RootMotion && (MovingAndInput || SpeedFast);	
}

void ADexCharacterBase::SmoothCharacterRotation(const FRotator& Target, const float TargetInterpSped, const float ActorInterpSpeed)
{
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, Target, LastDeltaTime, TargetInterpSped);
	FRotator NewCharacterRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, LastDeltaTime, ActorInterpSpeed);
	SetActorRotation(NewCharacterRotation);
}

float ADexCharacterBase::CalculateGroundRotationRate()
{
	if (UCurveFloat* Curve = CharacterMovementSettings.RotationRateCurve)
	{
		float CurveValue = Curve->GetFloatValue(GetMappedSpeed());
		float Mapped = DexMathUtility::RangeClamped(AimYawRate, 0.f, 300.f, 1.f, 3.f);
		return CurveValue * Mapped;
	}
	UE_LOG(LogDexCharacter, Warning, TEXT("CalculateGroundRotationRate failed to retrieve RotationRateCurve from CharacterMovementSettings. Is it set"));
	return 0.f;
}

void ADexCharacterBase::LimitRotation(const float AimYawMin, const float AimYawMax, const float InterpSpeed)
{
	FRotator ControlRotation = GetControlRotation();
	FRotator ControlActorDelta = DexMathUtility::NormalizedRotators(ControlRotation, GetActorRotation());
	bool bInRange = DexMathUtility::InRangeFloatFloat(ControlActorDelta.Yaw, AimYawMin, AimYawMax);
	if (false == bInRange)
	{
		float SmoothYaw = ControlActorDelta.Yaw > 0.f ? ControlRotation.Yaw + AimYawMax : ControlRotation.Yaw + AimYawMin;
		SmoothCharacterRotation(FRotator(0.f, SmoothYaw, 0.f), 0.f, InterpSpeed);
	}
}

void ADexCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	MovementState = ECharacterMovementState::Grounded;	

	float FallSplit = 0.f;
	if (UWorld* World = GetWorld())
	{
		float FallTime = World->GetTimeSeconds();
		FallSplit = FallTime - JumpInitiatedTime;

	}

	if (!FMath::IsNearlyZero(RollAttemptTime) || FallSplit > 1.25f)
	{
		Roll();
	}
}


void ADexCharacterBase::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	
	auto ResetJump = [&]() {
		bJumping = false;
	};
	
	bJumping = true;
	MovementState = ECharacterMovementState::InAir;

	if (UWorld* World = GetWorld())
	{
		JumpInitiatedTime = World->GetTimeSeconds();
		FTimerManager& TimerManger = World->GetTimerManager();
		FTimerHandle None;
		TimerManger.SetTimer(None, FTimerDelegate::CreateLambda(ResetJump), 0.5f, false, 0.f);		
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UDexCharacterAnimInst* AnimInst = Cast<UDexCharacterAnimInst>(MeshComp->GetAnimInstance()))
		{
			AnimInst->Jumped();
		}
	}
}

void ADexCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		EMovementMode NewMode = Movement->MovementMode;
		PreviousMovementState = MovementState;
		switch (NewMode)
		{
			case EMovementMode::MOVE_Walking:
			case EMovementMode::MOVE_NavWalking:
			{
				MovementState = ECharacterMovementState::Grounded;
			} break;
			case EMovementMode::MOVE_Falling:
			{
				MovementState = ECharacterMovementState::InAir;
			} break;
		}	
	}
	
}

void ADexCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityComponent();	
}

void ADexCharacterBase::PossessedBy(AController* C)
{
	Super::PossessedBy(C);
	if (ADexPlayerStateBase* State = GetState<ADexPlayerStateBase>())
	{
		SetPlayerState(State);
		InitAbilityComponent();
	}
}

ECharacterLocomotionState ADexCharacterBase::AllowedLocomotionState()
{
	auto AimingCrouchingFallback = [&]()
	{
		if (DesiredLocomotionState == ECharacterLocomotionState::Running ||
			DesiredLocomotionState == ECharacterLocomotionState::Sprinting)
		{
			return ECharacterLocomotionState::Running;
		}
		return ECharacterLocomotionState::Walking;
	};

	if (CharacterStance == ECharacterStance::Standing)
	{
		if (RotationMode == ECharacterRotationMode::AimingDirection)
		{
			return DesiredLocomotionState;
		}

		if (DesiredLocomotionState == ECharacterLocomotionState::Walking ||
			DesiredLocomotionState == ECharacterLocomotionState::Running)
		{
			return DesiredLocomotionState;
		}

		if (DesiredLocomotionState == ECharacterLocomotionState::Sprinting)
		{
			if (CanSprint())
			{
				return DesiredLocomotionState;
			}
			else
			{
				return ECharacterLocomotionState::Running;
			}
		}
	}

	if (CharacterStance == ECharacterStance::Crouching)
	{
		if (DesiredLocomotionState == ECharacterLocomotionState::Walking)
		{
			return DesiredLocomotionState;
		}

		return AimingCrouchingFallback();
	}		

	return ECharacterLocomotionState::Walking;
}

ECharacterLocomotionState ADexCharacterBase::ActualLocomotionState(ECharacterLocomotionState AllowedLocomotion)
{
	const float LocalSpeedOffset = 10.f;
	float LocalWalkSpeed = CharacterMovementSettings.WalkSpeed;
	float LocalRunSpeed = CharacterMovementSettings.RunSpeed;
	float LocalSprintSpeed = CharacterMovementSettings.SprintSpeed;

	if (Speed >= LocalRunSpeed + LocalSpeedOffset)
	{
		if (AllowedLocomotion == ECharacterLocomotionState::Sprinting)
		{
			return AllowedLocomotion;
		}
		return ECharacterLocomotionState::Running;
	}	
	if (Speed >= LocalWalkSpeed + LocalSpeedOffset)
	{
		return ECharacterLocomotionState::Running;
	}
	return ECharacterLocomotionState::Walking;
}

FCharacterMovementSettings ADexCharacterBase::GetTargetMovementSettings()
{
	if (nullptr == CharacterMovementStateData)
	{
		UE_LOG(LogDexCharacter, Warning, TEXT("Failed to get valid CharacterMovementStateData, is MovementModelData set?"));
		return FCharacterMovementSettings();
	}

	if (RotationMode == ECharacterRotationMode::VelocityDirection)
	{
		if (CharacterStance == ECharacterStance::Standing)
		{
			return CharacterMovementStateData->VelocityDirection.Standing;
		}

		if (CharacterStance == ECharacterStance::Crouching)
		{
			return CharacterMovementStateData->VelocityDirection.Crouching;			
		}
	}

	if (RotationMode == ECharacterRotationMode::LookingDirection)
	{
		if (CharacterStance == ECharacterStance::Standing)
		{
			return CharacterMovementStateData->LookingDirection.Standing;			
		}
		
		if (CharacterStance == ECharacterStance::Crouching)
		{
			return CharacterMovementStateData->LookingDirection.Crouching;						
		}
	}

	if (RotationMode == ECharacterRotationMode::AimingDirection)
	{
		if (CharacterStance == ECharacterStance::Standing)
		{
			return CharacterMovementStateData->AimingDirection.Standing;									
		}
		
		if (CharacterStance == ECharacterStance::Crouching)
		{
			return CharacterMovementStateData->AimingDirection.Crouching;												
		}
	}

	return FCharacterMovementSettings();
}

float ADexCharacterBase::GetMappedSpeed()
{
	float LocalWalkSpeed = CharacterMovementSettings.WalkSpeed;
	float LocalRunSpeed = CharacterMovementSettings.RunSpeed;
	float LocalSprintSpeed = CharacterMovementSettings.SprintSpeed;

	float MapA = DexMathUtility::RangeClamped(Speed, 0.f, LocalWalkSpeed, 0.f, 1.f);
	float MapB = DexMathUtility::RangeClamped(Speed, LocalWalkSpeed, LocalRunSpeed, 1.f, 2.f);
	float MapC = DexMathUtility::RangeClamped(Speed, LocalRunSpeed, LocalSprintSpeed, 2.f, 3.f);

	float A = Speed > LocalWalkSpeed ? MapB : MapA;
	float B = Speed > LocalRunSpeed ? MapC : A;
	return B;	
}

void ADexCharacterBase::UpdateCharacterMovment()
{
	// Update Character Movement Prior to checks
	CharacterMovementSettings = GetTargetMovementSettings();
	
	ECharacterLocomotionState Allowed = AllowedLocomotionState();
	ECharacterLocomotionState Actual = ActualLocomotionState(Allowed);
	LocomotionState = (Actual != LocomotionState) ? Actual : LocomotionState;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		if (Allowed == ECharacterLocomotionState::Walking)
		{
			Movement->MaxWalkSpeed = CharacterMovementSettings.WalkSpeed;			
		}
		if (Allowed == ECharacterLocomotionState::Running)
		{
			Movement->MaxWalkSpeed = CharacterMovementSettings.RunSpeed;			
		}
		if (Allowed == ECharacterLocomotionState::Sprinting)
		{
			Movement->MaxWalkSpeed = CharacterMovementSettings.SprintSpeed;			
		}

		if (UCurveVector* Curve = CharacterMovementSettings.MovementCurve)
		{
			float Mapped = GetMappedSpeed();
			FVector MovementCurveVector = Curve->GetVectorValue(Mapped);
			Movement->MaxAcceleration = MovementCurveVector.X;
			Movement->BrakingDecelerationWalking = MovementCurveVector.Y;
			Movement->GroundFriction = MovementCurveVector.Z;
		}
		else
		{
			UE_LOG(LogDexCharacter, Warning, TEXT("ADexCharacterBase::UpdateCharacterMovment() Unable to find CharacterMovementSettings.MovementCurve")); }
	}
}

bool ADexCharacterBase::CanSprint()
{
	if (bHasMovementInput)
	{
		if (RotationMode == ECharacterRotationMode::LookingDirection)
		{
			if (UCharacterMovementComponent* Movement = GetCharacterMovement())
			{
				FRotator AccelDirection = Movement->GetCurrentAcceleration().Rotation();
				FRotator Delta = DexMathUtility::NormalizedRotators(AccelDirection, GetControlRotation());
				return MovementInputAmount() > 0.9f && FMath::Abs(Delta.Yaw) < 50.f;
			}
		}

		if (RotationMode == ECharacterRotationMode::VelocityDirection)
		{
			return MovementInputAmount() > 0.9f;
		}
	}
	return false;
}

void ADexCharacterBase::ChangeViewMode(ECharacterViewMode NewView)
{
	if (ViewMode != NewView)
	{
		ViewMode = NewView;
		if (ViewMode == ECharacterViewMode::ThirdPerson)
		{
			if (RotationMode != ECharacterRotationMode::AimingDirection)
			{
				RotationMode = DesiredRotationMode;
			}
		}
		if (ViewMode == ECharacterViewMode::FirstPerson)
		{
			if (RotationMode == ECharacterRotationMode::VelocityDirection)
			{
				RotationMode = ECharacterRotationMode::LookingDirection;
			}
		}			
	}
}

void ADexCharacterBase::ChangeRotationMode(ECharacterRotationMode NewMode)
{
	if (RotationMode != NewMode)
	{
		RotationMode = NewMode;
		if (RotationMode == ECharacterRotationMode::VelocityDirection && ViewMode == ECharacterViewMode::FirstPerson)
		{
			ViewMode = ECharacterViewMode::ThirdPerson;
		}
	}
}

void ADexCharacterBase::ChangeLocomotionState(ECharacterLocomotionState NewState)
{
	if (LocomotionState != NewState)
	{
		LocomotionState = NewState;
	}
}