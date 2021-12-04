
#include "Camera/DexPlayerCameraAnimInst.h"
#include "Character/DexCharacterBase.h"

UDexPlayerCameraAnimInst::UDexPlayerCameraAnimInst(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	MovementState = ECharacterMovementState::Grounded;
	MovementAction = ECharacterMovementAction::None;
	RotationMode = ECharacterRotationMode::LookingDirection;
	LocomotionState = ECharacterLocomotionState::Walking;
	Stance = ECharacterStance::Standing;
	ViewMode = ECharacterViewMode::ThirdPerson;
}

void UDexPlayerCameraAnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// TODO Async loading
	if (const UDataTable* DT = CameraInstanceDataTable)
	{
		CameraInstanceData = DT->FindRow<FCameraInstanceData>(CameraInstanceDataRowName, TEXT("CameraAnimInstanceContext"));
	}	
}

void UDexPlayerCameraAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	if (ADexCharacterBase* Character = GetInstancePawn<ADexCharacterBase>())
	{
		MovementState = Character->MovementState;
		MovementAction = Character->MovementAction;
		RotationMode = Character->RotationMode;
		LocomotionState = Character->LocomotionState;
		Stance = Character->CharacterStance;
		ViewMode = Character->ViewMode;
		bRightShoulder = true;		
	}
	Super::NativeUpdateAnimation(DeltaSeconds);	
}

void UDexPlayerCameraAnimInst::GetCameraInstanceCurveData(float& CameraOffsetX, float& CameraOffsetY, float& CameraOffsetZ, float& PivotLagSpeedX, float& PivotLagSpeedY, float& PivotLagSpeedZ, float& RotationLagSpeed)
{
	if (CameraInstanceData)
	{
		const int32 RotationIndex = static_cast<int>(RotationMode);
		const int32 StanceIndex = static_cast<int>(Stance);
		const int32 LocomotionIndex = static_cast<int>(LocomotionState) - 1;

		if (CameraInstanceData->RotationCurvesData.IsValidIndex(RotationIndex))
		{
			FCameraInstanceRotationData A = CameraInstanceData->RotationCurvesData[RotationIndex];
			if (A.StateData.IsValidIndex(StanceIndex))
			{
				FCameraInstanceStanceData B = A.StateData[StanceIndex];
				if (B.MovementData.IsValidIndex(LocomotionIndex))
				{
					FCameraInstanceDirectionStateData C = B.MovementData[LocomotionIndex].DirectionData;
			
					CameraOffsetX = C.CameraOffsetX;
					CameraOffsetY = C.CameraOffsetY;
					CameraOffsetZ = C.CameraOffsetZ;
					PivotLagSpeedX = C.PivotLagSpeedX;
					PivotLagSpeedY = C.PivotLagSpeedY;
					PivotLagSpeedZ = C.PivotLagSpeedZ;
					RotationLagSpeed = C.RotationLagSpeed;
				}
			}
		}
	}
}