#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/DataTable.h"
#include "DexTimelineNative.h"
#include "Ability/DexAbilitySystemInterface.h"
#include "DexCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDexCharacter, Log, All);

class UCameraComponent;
class AController;
class UAnimMontage;
class UCapsuleComponent;
class UCurveFloat;
class UCurveVector;
class UDexAbilitySystemComponent;

UENUM(BlueprintType)
enum class ECharacterViewMode : uint8
{
	ThirdPerson,
	FirstPerson
};

UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	Standing,
	Crouching,
};

UENUM(BlueprintType)
enum class ECharacterRotationMode : uint8
{
	LookingDirection,
	VelocityDirection,
	AimingDirection,
};

UENUM(BlueprintType)
enum class ECharacterMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Roll, 
	Ragdoll,
};

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	None,
	Grounded,
	InAir,
};

UENUM(BlueprintType)
enum class ECharacterLocomotionState : uint8
{
	None,
	Walking,
	Running, 
	Sprinting
};

UENUM(BlueprintType)
enum class ECharacterOverlayState : uint8
{
	Default,
	Masculine,
	Feminine, 
	Inured,
	HandsTied,
	Rifle,
	Pistol,
	Pistol_2H,
	Bow,
	Torch,
	Binoculars,
	Box,
	Barrel,
};

USTRUCT(BlueprintType)
struct FDexCharacterRuntimeInputData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "DexCharacterRuntimeInputData")
	float ForwardAxisAmount;

	UPROPERTY(BlueprintReadOnly, Category = "DexCharacterRuntimeInputData")
	float RightAxisAmount;

	UPROPERTY(BlueprintReadOnly, Category = "DexCharacterRuntimeInputData")
	float LookAxisAmount;

	UPROPERTY(BlueprintReadOnly, Category = "DexCharacterRuntimeInputData")
	float UpAxisAmount;
};

USTRUCT(BlueprintType)
struct FDynamicMontageParams
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	UAnimSequenceBase* Animation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	float BlendInTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	float BlendOutTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	float PlayRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	float StartTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DynamicMontageParams")
	FName SlotName;
};

USTRUCT(BlueprintType)
struct FCharacterMovementSettings
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettings")
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettings")
	float RunSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettings")
	float SprintSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettings")
	UCurveVector* MovementCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettings")
	UCurveFloat* RotationRateCurve;
};

USTRUCT(BlueprintType)
struct FCharacterMovementSettingsStance
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettingsStance")
	FCharacterMovementSettings Standing;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettingsStance")
	FCharacterMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FCharacterMovementSettingsState : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettingsState")
	FCharacterMovementSettingsStance VelocityDirection;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettingsState")
	FCharacterMovementSettingsStance LookingDirection;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMovementSettingsState")
	FCharacterMovementSettingsStance AimingDirection;	
};

UCLASS()
class DEX_API ADexCharacterBase : public ACharacter, public IDexAbilitySystemInterface
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "DVCharacter|Input")
	FDexCharacterRuntimeInputData CharacterRuntimeInputData;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
		
protected:	

	// actor virtuals
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PIC) override;
	virtual void OnJumped_Implementation() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* C);

	// input
	void Forward(const float Amount);
	void Right(const float Amount);
	void Look(const float Amount);
	void Up(const float Amount);
	void Sprint();
	void Roll();	
	void PlayerInputMovement(bool bForward);
	void FixDiagonalGameplayValues(float& YOut, float& XOut);
	float MovementInputAmount();

	// rotation
	void UpdateGroundRotation();
	bool CanUpdateMovingRotation();
	void SmoothCharacterRotation(const FRotator& Target, const float TargetInterpSped, const float ActorInterpSpeed);
	float CalculateGroundRotationRate();
	void LimitRotation(const float AimYawMin, const float AimYawMax, const float InterpSpeed);

	// state driven functions
	ECharacterLocomotionState AllowedLocomotionState();
	ECharacterLocomotionState ActualLocomotionState(ECharacterLocomotionState AllowedLocomotion);
	FCharacterMovementSettings GetTargetMovementSettings();
	float GetMappedSpeed();
	void UpdateCharacterMovment();
	bool CanSprint();
	void ChangeViewMode(ECharacterViewMode NewView);
	void ChangeRotationMode(ECharacterRotationMode NewMode);
	void ChangeLocomotionState(ECharacterLocomotionState NewState);

	// data
	bool SetMovementModel();
	void SetFrameTimeData();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Input")
	FDataTableRowHandle MovementModelData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Input")
	UAnimMontage* RollDefault;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Input")	
	UAnimMontage* LandRoll_RH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Input")
	UAnimMontage* LandRoll_LH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Input")
	UAnimMontage* LandRoll_2H;	

	FRotator TargetRotation;
	FRotator LastVelocityRotation;
	FRotator LastMovementInputRotation;

	float LastDeltaTime;
	float RollAttemptTime;
	float JumpInitiatedTime;

	// util
	float GetAnimCurveValue(const FName& Name);
	UAnimMontage* GetRollAnimation();

	template<class T>
	T* GetAnimInstance() 
	{
		if (USkeletalMeshComponent* MeshComp = GetMesh())
		{
			return Cast<T>(MeshComp->GetAnimInstance());
		}
		return nullptr;
	}

	template<class T>
	T* GetState()
	{
		if (APlayerState* State = GetPlayerState())
		{
			return Cast<T>(State);
		}
		return nullptr;
	}

	template<class T>
	T* GetAbilutyComponent()
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	// ability
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	UDexAbilitySystemComponent* AbilitySystemComponent;

	void InitAbilityComponent();

public:

	// states
	bool bJumping;
	bool bHasMovementInput;
	bool bIsMoving;
	float Speed;
	float AimYawRate;
	float PreviousAimYaw;
	FVector Acceleration;
	FVector Velocity;
	FVector VelocityXY;
	FVector PreviousVelocity;
	ECharacterMovementState MovementState;
	ECharacterMovementState PreviousMovementState;	
	ECharacterLocomotionState LocomotionState;
	ECharacterLocomotionState DesiredLocomotionState;	
	ECharacterViewMode ViewMode;
	ECharacterStance CharacterStance;
	ECharacterRotationMode RotationMode;
	ECharacterRotationMode DesiredRotationMode;
	ECharacterMovementAction MovementAction;
	ECharacterOverlayState OverlayState;

	FCharacterMovementSettings CharacterMovementSettings;
	FCharacterMovementSettingsState* CharacterMovementStateData;
};
