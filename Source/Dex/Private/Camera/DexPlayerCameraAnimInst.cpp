
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
		const FCameraInstanceDirectionStateData& CameraInstanceDirectionStateData = CameraInstanceData->RotationCurvesData[static_cast<int>(RotationMode)].StateData[static_cast<int>(Stance)].MovementData[static_cast<int>(LocomotionState)].DirectionData;
		CameraOffsetX = CameraInstanceDirectionStateData.CameraOffsetX;
		CameraOffsetY = CameraInstanceDirectionStateData.CameraOffsetY;
		CameraOffsetZ = CameraInstanceDirectionStateData.CameraOffsetZ;
		PivotLagSpeedX = CameraInstanceDirectionStateData.PivotLagSpeedX;
		PivotLagSpeedY = CameraInstanceDirectionStateData.PivotLagSpeedY;
		PivotLagSpeedZ = CameraInstanceDirectionStateData.PivotLagSpeedZ;
		RotationLagSpeed = CameraInstanceDirectionStateData.RotationLagSpeed;
	}
}