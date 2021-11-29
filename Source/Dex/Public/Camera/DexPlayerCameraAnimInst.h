#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Character/DexMovementData.h"
#include "Character/DexPlayerCameraManager.h"
#include "Character/DexCharacterBase.h"
#include "UObject/SoftObjectPtr.h"
#include "DexPlayerCameraAnimInst.generated.h"

USTRUCT(BlueprintType)
struct FCameraInstanceDirectionStateData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float CameraOffsetX;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float CameraOffsetY;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float CameraOffsetZ;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float PivotLagSpeedX;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float PivotLagSpeedY;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float PivotLagSpeedZ;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraMangerCurveData")
	float RotationLagSpeed;
};

UENUM(BlueprintType)
enum class ECameraInstanceRotationState : uint8
{
	Looking,
	Velocity,
	Aiming,
	Num,
};

UENUM(BlueprintType)
enum class ECameraInstanceStanceState : uint8
{
	Standing,
	Crouching,
	Num,
};

UENUM(BlueprintType)
enum class ECameraInstanceMovementState : uint8
{
	Walking,
	Running,
	Sprinting, 
	Num,
};

USTRUCT(BlueprintType)
struct FCameraInstanceMovementStateData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceMovementStateData")
	ECameraInstanceMovementState MovementState;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceMovementStateData")		
	FCameraInstanceDirectionStateData DirectionData;		
};

USTRUCT(BlueprintType)
struct FCameraInstanceStanceData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FameraInstanceStanceData")
	ECameraInstanceStanceState StanceState;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceStanceData")		
	TArray<FCameraInstanceMovementStateData> MovementData;		
	
};

// TODO(Kyle): Change to Datatable? Currently all hand typed in Blueprint class.
USTRUCT(BlueprintType)
struct FCameraInstanceRotationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceRotationData")
	ECameraInstanceRotationState RotationState;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceRotationData")		
	TArray<FCameraInstanceStanceData> StateData;		
};

USTRUCT(BlueprintType)
struct FCameraInstanceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraInstanceData")
	TArray<FCameraInstanceRotationData> RotationCurvesData;
};


UCLASS()
class DEX_API UDexPlayerCameraAnimInst : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")
	EMovementState MovementState;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	EMovementAction MovementAction;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	ERotationMode RotationMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	EMovementGait Gait;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	EStance Stance;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	EViewMode ViewMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "CameraAnimInstance")		
	bool bRightShoulder;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraAnimInstance")		
	FName CameraInstanceDataRowName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraAnimInstance")		
	UDataTable* CameraInstanceDataTable;
	
	UFUNCTION(BlueprintPure, Category = "CameraAnimInstance")
	void GetCameraInstanceCurveData(float& CameraOffsetX, float& CameraOffsetY, float& CameraOffsetZ, float& PivotLagSpeedX, float& PivotLagSpeedY, float& PivotLagSpeedZ, float& RotationLagSpeed);
	
	FCameraInstanceData* CameraInstanceData;	
	
	template<typename T>
	T* GetInstancePawn()
	{
		if (ADexPlayerCameraManager* PlayerCameraManager = Cast<ADexPlayerCameraManager>(GetOwningActor()))
		{
			return Cast<T>(PlayerCameraManager->GetControlledPawn<T>());
		}
		return nullptr;
	}
	
	virtual void NativeBeginPlay() override;	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
