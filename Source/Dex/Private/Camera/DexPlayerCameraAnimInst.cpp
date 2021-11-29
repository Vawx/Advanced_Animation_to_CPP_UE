
#include "Camera/DexPlayerCameraAnimInst.h"
#include "Character/DexCharacterBase.h"
#include "Character/DexCharacterMovementData.h"

UDexPlayerCameraAnimInst::UDexPlayerCameraAnimInst(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

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
		const UDexCharacterMovementData* Movement = Character->GetMovementData();
		if (Movement->Gait != EMovementGait::None)
		{
			MovementState = Movement->MovementState;
			MovementAction = Movement->MovementAction;
			RotationMode = Movement->RotationMode;
			Gait = Movement->Gait;
			Stance = Movement->Stance;
			ViewMode = Movement->ViewMode;
			bRightShoulder = Character->bRightShoulder;
		}
	}
	
	Super::NativeUpdateAnimation(DeltaSeconds);	
}

void UDexPlayerCameraAnimInst::GetCameraInstanceCurveData(float& CameraOffsetX, float& CameraOffsetY, float& CameraOffsetZ, float& PivotLagSpeedX, float& PivotLagSpeedY, float& PivotLagSpeedZ, float& RotationLagSpeed)
{
	if (CameraInstanceData)
	{
		const FCameraInstanceDirectionStateData& CameraInstanceDirectionStateData = CameraInstanceData->RotationCurvesData[static_cast<int>(RotationMode)].StateData[static_cast<int>(Stance)].MovementData[static_cast<int>(Gait)].DirectionData;
		CameraOffsetX = CameraInstanceDirectionStateData.CameraOffsetX;
		CameraOffsetY = CameraInstanceDirectionStateData.CameraOffsetY;
		CameraOffsetZ = CameraInstanceDirectionStateData.CameraOffsetZ;
		PivotLagSpeedX = CameraInstanceDirectionStateData.PivotLagSpeedX;
		PivotLagSpeedY = CameraInstanceDirectionStateData.PivotLagSpeedY;
		PivotLagSpeedZ = CameraInstanceDirectionStateData.PivotLagSpeedZ;
		RotationLagSpeed = CameraInstanceDirectionStateData.RotationLagSpeed;
	}
}