#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "DexMovementData.h"
#include "GameFramework/Character.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "DexTimelineNative.h"

#include "DexCharacterMovementData.generated.h"

#define ENABLE_CHARACTER_DEBUG 0

class UDataTable;

UCLASS()
class UDexCharacterMovementData : public UObject
{
	GENERATED_BODY()

public:

	UDexCharacterMovementData();

	virtual void PostInitProperties() override;

	// Default Settings and content
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	ERotationMode DesiredRotationMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	EMovementGait DesiredGait;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	EStance DesiredStance;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UDataTable* MovementModel;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleTraceParams GroundedTraceSettings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleTraceParams AutomaticTraceSettings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleTraceParams FallingTraceSettings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FDexTimeline MantleTimeline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_2m_Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_1m_Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_1m_RH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_1m_LH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_1m_2H;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMantleAsset Mantle_1m_Box;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	FMovementSettings CurrentMovementSettings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* LandRollDefault;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* LandRollRightHand;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* LandRollLeftHand;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* LandRollTwoHand;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UCurveVector* MantleOneMeter;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UCurveVector* MantleTwoMeter;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetBackUpDefault;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetBackUpLH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetBackUp2H;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetBackUpRH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetFrontUpDefault;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetFrontUpLH;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetFrontUp2H;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	UAnimMontage* GetFrontUpRH;

	// Runtime values
	UPROPERTY()
	bool bBreakFall;

	UPROPERTY()
	bool bSprintHeld;

	UPROPERTY()
	FVector Acceleration;

	UPROPERTY()
	bool bIsMoving;

	UPROPERTY()
	bool bHasMovementInput;

	UPROPERTY()
	FRotator LastVelocityRotation;

	UPROPERTY()
	FRotator LastMovementInputRotation;

	UPROPERTY()
	float Speed;

	UPROPERTY()
	float MovementInputAmount;

	UPROPERTY()
	float AimYawRate;

	UPROPERTY()
	EMovementState MovementState;

	UPROPERTY()
	EMovementState PreviousMovementState;

	UPROPERTY()
	EMovementAction MovementAction;

	UPROPERTY()
	ERotationMode RotationMode;

	UPROPERTY()
	EMovementGait Gait;

	UPROPERTY()
	EStance Stance;

	UPROPERTY()
	EViewMode ViewMode;

	UPROPERTY()
	EOverlayState OverlayState;

	FMovementSettingsState* MovementData;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	FRotator InAirRotation;

	UPROPERTY()
	float YawOffset;

	UPROPERTY()
	FMantleParams MantleParams;

	UPROPERTY()
	FComponentAndTransform MantleLedgeLS;

	UPROPERTY()
	FTransform MantleTarget;

	UPROPERTY()
	FTransform MantleActualStartOffset;

	UPROPERTY()
	FTransform MantleAnimationStartOffset;

	UPROPERTY()
	bool bRagdollOnGround;

	UPROPERTY()
	bool bRagdollFaceUp;

	UPROPERTY()
	FVector LastRagdollVelocity;

	UPROPERTY()
	FVector PreviousVelocity;

	UPROPERTY()
	float PreviousAimYaw;

	UPROPERTY()
	FMantleAsset CharacterMantleAsset;

	UPROPERTY()
	FComponentAndTransform Mantle;

	// class
	virtual void Tick(float DeltaTime);
		
protected:

	void SetEssentialValues(ACharacter* Character, const float DeltaTime);
	void UpdateMovementState(ACharacter* OwningCharacter, const float DeltaTime);	
	const EMovementGait GetAllowedGait(ACharacter* OwningCharacter) const;		
	EMovementGait GetActualGait(const EMovementGait G);
	const bool CanSprint(ACharacter* OwningCharacter) const;
	void UpdateDynamicMovementSettings(const EMovementGait G, ACharacter* OwningCharacter);
	const FMovementSettings& GetTargetMovementSettings() const;	
	const float GetMappedSpeed() const;
	void UpdateGroundedRotation(ACharacter* OwningCharacter, const float DeltaTime);
	void SmoothCharacterRotation(ACharacter* OwningPlayer, const FRotator& Target, const float TargetLerpSpeed, const float ActorInterpSpeed, const float DeltaTime);
	const float GetAnimCurveValue(ACharacter* OwningCharacter, const FName& CurveName);
	void LimitRotation(ACharacter* OwningCharacter, const float AimYawMin, const float AimYawMax, const float InterpSpeed, const float DeltaTime);
	void UpdateInAirRotation(ACharacter* OwningCharacter, const float DeltaTime);
	const bool MantleCheck(ACharacter* OwningCharacter, const FMantleTraceParams& TraceParams);
	const FVector GetPlayerMovementInput(ACharacter* OwningCharacter);
	const FVector GetCapsuleBaseLocation(ACharacter* OwningCharacter, const float ZOffset);
	const FVector GetCapsuleLocationFromBase(ACharacter* OwningCharacter, const FVector& BaseLocation, const float ZOffset);
	const bool CapsuleHasRoomCheck(ACharacter* OwningCharacter, const UCapsuleComponent* Capsule, const FVector& TargetLocation, const float HeightOffset, const float RadiusOffset);
	void MantleStart(ACharacter* OwningCharacter, const float MantleHeight, FComponentAndTransform& MantleLedgeWorldSpace, const EMantleType Type);
	void MantleUpdate(ACharacter* OwningCharacter, const float DeltaTime);
	void MantleEnd(ACharacter* OwningCharacter);
	const bool SetActorLocationAndRotationUpdateTarget(ACharacter* OwningCharacter, FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult* OutSweepHitResult);
	

	const FMantleAsset& GetMantleAsset(const EMantleType Type);
	void MantleTimelineUpdate(float Delta);
	void MantleTimelineFinished();

	ACharacter* GetOwningCharacter()
	{
		return Cast<ACharacter>(const_cast<ACharacter*>(CharacterOwner.Get(false)));
	}
	TWeakObjectPtr<ACharacter> CharacterOwner;
};