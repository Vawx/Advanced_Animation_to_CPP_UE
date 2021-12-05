#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/DexCharacterBase.h"
#include "DexCharacterAnimInst.generated.h"

#define DEXCHARACTER_ANIMINST_DEBUG_IK 0
#define DEXCHARACTER_ANIMINST_DEBUG_LOCK_OFFSET 0
#define DEXCHARACTER_ANIMINST_DEBUG_LAND_PREDICTION 0

class UCurveVector;
class UCurveFloat;

USTRUCT()
struct FFootLockingData
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	float* FootLockAlpha; 
	FVector* FootLockLocation;
	FRotator* FootLockRotation;
	FName* IK;
	FName* Lock;
	FName* EnableCurve;
};

USTRUCT()
struct FFootOffsetData
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	FName* EnableCurve;
	FName* IK;
	FName* Root;
	FVector* LocationTarget;
	FVector* LocationOffset; 
	FRotator* RotationOffset;
};

USTRUCT(BlueprintType)
struct FVelocityBlend
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, Category = "VelocityBlend")
	float F;

	UPROPERTY(BlueprintReadOnly, Category = "VelocityBlend")
	float B;

	UPROPERTY(BlueprintReadOnly, Category = "VelocityBlend")
	float L;

	UPROPERTY(BlueprintReadOnly, Category = "VelocityBlend")
	float R;	
};

USTRUCT(BlueprintType)
struct FLeanAmount
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "LeanAmount")
	float LR;

	UPROPERTY(BlueprintReadOnly, Category = "LeanAmount")
	float FB;
};

USTRUCT(BlueprintType)
struct FTurnInPlaceAsset
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	UAnimSequence* Animation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	float AnimationAngle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	float PlayRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	FName SlotName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TurnInPlaceAsset")
	bool bScaleTurnAngle;
};

UENUM(BlueprintType)
enum class ECharacterMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType)
enum class ECharacterHipsDirection : uint8
{
	F,
	B,
	RF,
	RB,
	LF,
	LB
};

UCLASS()
class DEX_API UDexCharacterAnimInst : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	bool bJumping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	bool bHasMovementInput;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float FallSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float EnableAimOffset;	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float BasePoseNormal;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float BasePoseCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float SpineAdd;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float HeadAdd;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmLAdd;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmRAdd;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float HandR;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float HandL;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float EnableHandIKL;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float EnableHandIKR;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmLLS;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmLMS;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmRLS;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ArmRMS;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float JumpPlayRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	ECharacterMovementState MovementState;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")		
	ECharacterMovementState PreviousMovementState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	ECharacterLocomotionState LocomotionState;

	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	ECharacterRotationMode RotationMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	ECharacterStance CharacterStance;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	ECharacterViewMode ViewMode;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")		
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")		
	UCurveFloat* DiagonalScaleAmountCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")
	UCurveFloat* StrideBlendWalk;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")			
	UCurveFloat* StrideBlendRun;	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")				
	UCurveFloat* StrideBlendCrouchWalk;		

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")				
	UCurveVector* YawOffsetFrontBack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")					
	UCurveVector* YawOffsetLeftRight;	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")					
	UCurveFloat* LandPredictionCurve;	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")					
	UCurveFloat* LeanInAirCurve;	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")			
	float WalkRunBlend;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")			
	float StrideBlend;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")			
	float StandingPlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")			
	float CrouchingPlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")				
	ECharacterMovementDirection MovementDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	FRotator AimingRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	float FYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	float BYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	float LYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	float RYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")					
	float ElapsedDelayTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")						
	bool RotateL;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")						
	bool RotateR;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")							
	float DiagonalScaleAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")							
	ECharacterHipsDirection HipsDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")								
	FVector RelativeAccelerationAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	bool bShouldMove;	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootLockLLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator FootLockLRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float FootLockLAlpha;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootLockRLocation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator FootLockRRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float FootLockRAlpha;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootOffsetLLocation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootOffsetLTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator FootOffsetLRotation;
		
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootOffsetRLocation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector FootOffsetRTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator FootOffsetRRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector PelvisOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float PelvisAlpha;

	void Jumped();
	 
protected:

	float LastDelta;

	FVector Acceleration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")			
	FTurnInPlaceAsset TurnInPlaceLeft90;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")			
	FTurnInPlaceAsset TurnInPlaceRight90;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")			
	FTurnInPlaceAsset TurnInPlaceLeft180;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation State Data")			
	FTurnInPlaceAsset TurnInPlaceRight180;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")			
	FVelocityBlend VelocityBlend;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")				
	FLeanAmount LeanAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	bool bRotateLeft;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	bool bRotateRight;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float RotateRate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float RotationScale;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float AimYawRate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector2D AimingAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator SmoothedAimingRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FRotator SpineRotaion;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	FVector2D SmoothedAimingAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")
	float AimSweepTime;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float InputYawOffsetTime;	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	FVector MovementInput;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float LeftYawTime;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float RightYawTime;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")	
	float ForwardYawTime;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")		
	float TurnAngle;	

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Data")		
	float LandPrediction;

	FTurnInPlaceAsset TurnInPlaceAsset;
	
	// members
	virtual void NativeBeginPlay() override;	
	virtual void NativeUpdateAnimation(float DeltaTime) override;
		
	template<class T>
	T* GetCharacter()
	{
		return Cast<T>(TryGetPawnOwner());
	}

	void UpdateMovementValues();
	void UpdateRotationValues();
	FVelocityBlend InterpVelocityBlend(const FVelocityBlend& Current, const FVelocityBlend& Target, const float InterpSpeed);
	FVelocityBlend CalculateVelocityBlend();
	ECharacterMovementDirection GetMovementDirection();
	ECharacterMovementDirection CalculateQuadrant(ECharacterMovementDirection Dir, const float FRThreshold, const float FLThreshold, const float BRThreshold, const float BLThreshold, const float Buffer, const float Angle); ;
	bool AngleInRange(const float Angle, const float Min, const float Max, const float Buffer, const bool bIncreaseBuffer);
	bool ShouldMove();
	void UpdateAiming();
	void UpdateLayerValues();
	void UpdateFeet();

	UFUNCTION(BlueprintCallable, Category = "Dex Character Anim Inst")
	bool PlayTransitionAnimation(FDynamicMontageParams Params);

	UFUNCTION(BlueprintCallable, Category = "Dex Character Anim Inst")
	void SetHipsDirection(ECharacterHipsDirection Dir);

	UFUNCTION(BlueprintCallable, Category = "Dex Character Anim Inst")
	void SetFootLocation(bool bLeft, FVector Location, FRotator Rotation, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Dex Character Anim Inst")
	void SetFootOffset(bool bLeft, FVector Location, FRotator Rotation);

};
