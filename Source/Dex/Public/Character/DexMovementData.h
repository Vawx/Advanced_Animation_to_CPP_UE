#pragma once

#include "UObject/WeakObjectPtrTemplates.h"
#include "Engine/DataTable.h"
#include "DexMovementData.generated.h"

class UPrimitiveComponent;
class UWorld;
class UAnimSequenceBase;
class UAnimMontage;
class UCurveVector;
class UCurveFloat;

UENUM(BlueprintType)
enum class EMovementGait : uint8
{
	None,
	Walking,
	Running, 
	Sprinting,
}; 

UENUM(BlueprintType)
enum class EMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp,
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll,
};

UENUM(BlueprintType)
enum class EOverlayState : uint8
{
	Default,
	Masculine,
	Feminine,
	Injured,
	HandsTied,
	Rifle,
	PistolOneHand,
	PistolTwoHand,
	Bow,
	Torch,
	Binoculars,
	Box, 
	Barrel,
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	AimingDirection,
};

UENUM(BlueprintType)
enum class EStance : uint8
{
	Standing,
	Crouching,
};

UENUM(BlueprintType)
enum class EViewMode : uint8
{
	ThirdPerson,
	FirstPerson,
};

UENUM(BlueprintType)
enum class EFootStepType : uint8
{
	Step,
	Run,
	Jump,
	Land,
};

UENUM(BlueprintType)
enum class EGroundedEntryState : uint8
{
	None,
	Roll,
};

UENUM(BlueprintType)
enum class EHipsDirection : uint8
{
	Front,
	Back,
	RightFront,
	RightBack,
	LeftFront,
	LeftBack,
};

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	HighMantle,
	LowMantle,
	FallingCatch,
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	Forward,
	Right,
	Left, 
	Backward,
};

USTRUCT(BlueprintType)
struct FComponentAndTransform 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ComponentAndTransform")
	FTransform Transform;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ComponentAndTransform")
	TWeakObjectPtr<UPrimitiveComponent> Component;
	
public:
	
	void Reset();
	bool Valid();
	
	FComponentAndTransform() {};
	FComponentAndTransform(FTransform InTransform, UPrimitiveComponent* InComponent);
};

USTRUCT(BlueprintType)
struct FCameraSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettings")
	float TargetArmLength;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettings")
	float LagSpeed;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettings")
	float RotationLagSpeed;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettings")
	bool bDoCollisionTest;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettings")
	FVector SocketOffset;
};

USTRUCT(BlueprintType)
struct FCameraSettingsGait
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsGait")
	FCameraSettings Walking;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsGait")		
	FCameraSettings Running;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsGait")		
	FCameraSettings Sprinting;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsGait")		
	FCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FCameraSettingsState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsState")
	FCameraSettingsGait VelocityDirection;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsState")
	FCameraSettingsGait LookingDirection;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraSettingsState")		
	FCameraSettingsGait AimingDirection;	
};

USTRUCT(BlueprintType)
struct FDynamicMontageParams
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DynamicMontageParams")
	UAnimSequenceBase* Animation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DynamicMontageParams")
	float BlendInTime;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DynamicMontageParams")
	float BlendOutTime;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DynamicMontageParams")
	float PlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DynamicMontageParams")
	float StartTime;
	
public:
	FDynamicMontageParams() {};
};

USTRUCT(BlueprintType)
struct FLeanAmount
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "LeanAmount")
	float LR;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "LeanAmount")
	float FB;
	
	FLeanAmount() {}
	FLeanAmount(float LeftRight, float FrontBack) : LR(LeftRight), FB(FrontBack) {}
};

USTRUCT(BlueprintType)
struct FMantleAsset
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	UAnimMontage* AnimMontage;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	UCurveVector* PositionCorrectionCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	FVector StartingOffset;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float LowHeight;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float LowPlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float LowStartPosition;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float HighHeight;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float HighPlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleAsset")
	float HighStartPosition;
	
public:
	
	FMantleAsset() {};
};

USTRUCT(BlueprintType)
struct FMantleParams
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleParams")
	UAnimMontage* AnimMontage;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleParams")
	UCurveVector* PositionCorrectionCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleParams")
	FVector StartingOffset;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleParams")
	float StartingPosition;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleParams")
	float PlayRate;
	
public:
	
	FMantleParams() {};
};

USTRUCT(BlueprintType)
struct FMantleTraceParams
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleTraceParams")
	float MaxLedgeHeight;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleTraceParams")
	float MinLedgeHeight;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleTraceParams")
	float ReachDistance;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleTraceParams")
	float ForwardTraceRadius;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MantleTraceParams")
	float DownwardTraceRadius;
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettings")
	UCurveVector* MovementCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettings")
	UCurveFloat* RotationRateCurve;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettings")
	float WalkSpeed;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettings")
	float RunSpeed;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettings")
	float SprintSpeed;
	
public:
	
	FMovementSettings() {};
};

USTRUCT(BlueprintType)
struct FMovementSettingsStance
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettingsStance")
	FMovementSettings Standing;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettingsStance")
	FMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FMovementSettingsState : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettingsState")
	FMovementSettingsStance VelocityDirection;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettingsState")
	FMovementSettingsStance LookingDirection;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MovementSettingsState")
	FMovementSettingsStance AimingDirection;
};

USTRUCT(BlueprintType)
struct FRotationInPlaceAsset
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	UAnimSequenceBase* Animation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	FName SlotName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	float SlowTurnRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	float FastTurnRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	float SlowPlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RotationInPlaceAsset")
	float FastPlayRate;
	
public:
	FRotationInPlaceAsset() {};
};

USTRUCT(BlueprintType)
struct FTurnInPlaceAsset
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	UAnimSequenceBase* Animation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	FName SlotName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	float AnimationAngle;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	float PlayRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	bool bScaleTurnAngle;
	
	FTurnInPlaceAsset() {};
};

USTRUCT(BlueprintType)
struct FVelocityBlend
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VelocityBlend")
	float F;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VelocityBlend")
	float B;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VelocityBlend")
	float L;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "VelocityBlend")
	float R;
};