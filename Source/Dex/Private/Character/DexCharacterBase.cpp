#include "Character/DexCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogDexCharacter);

ADexCharacterBase::ADexCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	FirstPersonFOV = 90.f;
	ThirdPersonFOV = 90.f;
	LookLeftRightRate = 1.25f;
	LookUpDownRate = 1.25f;
}

void ADexCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MovementData = NewObject<UDexCharacterMovementData>(this);
}

void ADexCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UDexCharacterMovementData* Data = GetMovementData())
	{
		Data->Tick(DeltaTime);
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
	}

	Super::SetupPlayerInputComponent(PIC);
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
	AddControllerYawInput(Amount * LookLeftRightRate);
}

void ADexCharacterBase::Up(const float Amount)
{
	CharacterRuntimeInputData.UpAxisAmount = Amount;
	AddControllerPitchInput(Amount * LookUpDownRate);	
}

void ADexCharacterBase::PlayerInputMovement(bool bForward)
{
	if (AController* Cont = GetController())
	{
		float X = 0.f;
		float Y = 0.f;
		FixDiagonalGameplayValues(Y, X);

		if (const UDexCharacterMovementData* Data = GetMovementData())
		{
			const EMovementState MovementState = Data->MovementState;
			if (MovementState == EMovementState::Grounded || MovementState == EMovementState::InAir)
			{
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
	}
}

void ADexCharacterBase::FixDiagonalGameplayValues(float& YOut, float& XOut)
{
	const float A = FMath::Abs(CharacterRuntimeInputData.ForwardAxisAmount);
	const float B = UKismetMathLibrary::MapRangeClamped(A, 0.f, 0.6f, 1.f, 1.2f);
	YOut = FMath::Clamp(CharacterRuntimeInputData.ForwardAxisAmount * B, -1.f, 1.f);
	
	const float C = FMath::Abs(CharacterRuntimeInputData.RightAxisAmount);
	const float D = UKismetMathLibrary::MapRangeClamped(C, 0.f, 0.6f, 1.f, 1.2f);
	XOut = FMath::Clamp(CharacterRuntimeInputData.RightAxisAmount * D, -1.f, 1.f);
}

