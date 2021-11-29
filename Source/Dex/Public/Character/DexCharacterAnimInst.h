#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/DexMovementData.h"
#include "DexCharacterAnimInst.generated.h"

class UCurveVector;


UCLASS()
class DEX_API UDexCharacterAnimInst : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	FRotator AimingRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	FVector Velocity;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	FVector RelativeVelocityDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	FVector Acceleration;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	FVector MovementInput;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	bool bIsMoving;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	bool bHasMovementInput;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	float MovementeInputAmount;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	float AimYawRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	float ZoomAmount;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EMovementState MovementState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EMovementState PreviousMovementState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EMovementAction MovementAction;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	ERotationMode RotationMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EMovementGait Gait;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EStance Stance;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EViewMode ViewMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|CharacterInformation")
	EOverlayState OverlayState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	EGroundedEntryState GroundedEntryState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	EMovementDirection MovementDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	EHipsDirection HipsDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	FVector RelativeAccerationAmount;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	bool bShouldMove;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	bool bRotateLeft;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	bool bRotateRight;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	bool bPivot;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float RotationRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float RotationScale;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float DiagonalScaleAmount;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float WalkRunBlend;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float StandingPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float CrouchingPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float StrideBlend;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	FVelocityBlend VelocityBlend;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	FLeanAmount LeanAmount;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float FYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float BYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float LYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Grounded")
	float RYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|InAir")
	bool bJumped;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|InAir")
	float JumpPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|InAir")
	float FallSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|InAir")
	float LandPrediction;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	FRotator SmoothedAimingRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	FRotator SpineRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	FVector2D AimingAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	FVector2D SmoothedAimingAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	float AimSweepTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	float InputYawOffsetTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	float ForwardYawTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	float LeftYawTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Aiming")
	float RightYawTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Ragdoll")
	float FlailRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	int OverlayOverrideState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float EnableAimOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float BasePoseN;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float BasePoseCLF;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_L;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_L_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_L_LS;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_L_MS;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_R;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_R_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_R_LS;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Aim_R_MS;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Hand_L;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Hand_R;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Legs;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Legs_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Pelvis;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Pelvis_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Spine;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Spine_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Head;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Head_Add;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Enable_HandIK_L;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|LayerBlending")
	float Enable_HandIK_R;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	float FootLock_L_Alpha;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	float FootLock_R_Alpha;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FVector FootLock_L_Location;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FVector FootLock_R_Location;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FRotator FootLock_L_Rotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FRotator FootLock_R_Rotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FVector FootOffset_L_Location;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FVector FootOffset_R_Location;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FRotator FootOffset_L_Rotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FRotator FootOffset_R_Rotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	FVector PelvisOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|FootIK")
	float PelvisAlpha;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float TurnCheckMinAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float Turn180Threshold;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float AimYawRateLimit;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float ElapsedDelayTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float MinAngleDelay;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|TurnInPlace")
	float MaxAngleDelay;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset N_TurnIP_L90;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset N_TurnIP_R90;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset N_TurnIP_L180;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset N_TurnIP_R180;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset CLF_TurnIP_L90;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset CLF_TurnIP_R90;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset CLF_TurnIP_L180;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|TurnInPlace")
	FTurnInPlaceAsset CLF_TurnIP_R180;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float RotationMinThreshold;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float RotateMaxThreshold;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float AimYawRateMinRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float AimYawRateMaxRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float MinPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|RotateInPlace")
	float MaxPlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* DiagonalScaleAmountCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* StrideBlend_N_Walk;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* StrideBlend_N_Run;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* StrideBlend_C_Walk;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* LandPredictionCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveFloat* LeanInAirCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveVector* YawOffset_FB;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|BlendCurves")
	UCurveVector* YawOffset_LR;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float AnimationWalkSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float AnimationRunSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float AnimationSprintSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float AnimationCrouchSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float VelocityBlendInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float GroundedLeanInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float InAirLeanInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float SmoothedAimingRotationInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float InputYawOffsetInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float TriggerPivotSpeedLimit;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float FootHeight;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float IK_TraceDistanceAboveFeet;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterAnimInstance|Config")
	float IK_TraceDistanceBelowFeet;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|Assets")
	UAnimSequence* TransitionLeft;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterAnimInstance|Assets")
	UAnimSequence* TransitionRight;

protected:

	virtual void NativeBeginPlay() override;	
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	
};
