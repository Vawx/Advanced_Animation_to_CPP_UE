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

	/** Character values on tick*/
	void SetEssentialValues(ACharacter* Character, const float DeltaTime);

	/** Check and update movement state on tick */
	void UpdateMovementState(ACharacter* OwningCharacter, const float DeltaTime);	

	/** Get what gait is currently allowed given other metrics */
	const EMovementGait GetAllowedGait(ACharacter* OwningCharacter) const;		

	/** Get the actual gait happening */
	EMovementGait GetActualGait(const EMovementGait G);

	/** Can sprint happen */
	const bool CanSprint(ACharacter* OwningCharacter) const;

	/** Updates the dynamic animation movement settings based on gait */
	void UpdateDynamicMovementSettings(const EMovementGait G, ACharacter* OwningCharacter);

	/** Gets the data for target movement */
	const FMovementSettings& GetTargetMovementSettings() const;	

	/** Maps the speed based on walk, run, and sprint values*/
	const float GetMappedSpeed() const;

	/** Updates the rotation while on the ground */
	void UpdateGroundedRotation(ACharacter* OwningCharacter, const float DeltaTime);

	/** Smooths the character rotation with lerp*/
	void SmoothCharacterRotation(ACharacter* OwningPlayer, const FRotator& Target, const float TargetLerpSpeed, const float ActorInterpSpeed, const float DeltaTime);

	/** Utility to get a curve value on the Owning Character */
	const float GetAnimCurveValue(ACharacter* OwningCharacter, const FName& CurveName);

	/** Limit the rotation of the OwningCharacter based on Min and Max */
	void LimitRotation(ACharacter* OwningCharacter, const float AimYawMin, const float AimYawMax, const float InterpSpeed, const float DeltaTime);

	/** Updates the rotation of the OwningCharacter while in the air */
	void UpdateInAirRotation(ACharacter* OwningCharacter, const float DeltaTime);

	/** Check while in the air to see if there is a mantle ledge nearby */
	const bool MantleCheck(ACharacter* OwningCharacter, const FMantleTraceParams& TraceParams);

	/** Get the OwningCharacter input for movment */
	const FVector GetPlayerMovementInput(ACharacter* OwningCharacter);

	/** Get the OwningCharacter capsule location*/
	const FVector GetCapsuleBaseLocation(ACharacter* OwningCharacter, const float ZOffset);

	/** Get the OwningCharacter capsule location based on the base value */
	const FVector GetCapsuleLocationFromBase(ACharacter* OwningCharacter, const FVector& BaseLocation, const float ZOffset);

	/** Check to see if OwningPlayer has room with the current capsule size */
	const bool CapsuleHasRoomCheck(ACharacter* OwningCharacter, const UCapsuleComponent* Capsule, const FVector& TargetLocation, const float HeightOffset, const float RadiusOffset);

	/** Start to mantle */
	void MantleStart(ACharacter* OwningCharacter, const float MantleHeight, FComponentAndTransform& MantleLedgeWorldSpace, const EMantleType Type);

	/** Update every frame while mantling */
	void MantleUpdate(ACharacter* OwningCharacter, const float DeltaTime);

	/** Mantle clean up after it is done. */
	void MantleEnd(ACharacter* OwningCharacter);

	/** Utility to set location and rotation on OwningCharacter */
	const bool SetActorLocationAndRotationUpdateTarget(ACharacter* OwningCharacter, FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult* OutSweepHitResult);

	/** Update during ragdoll */
	void RagdollUpdate(ACharacter* OwningCharacter, const float DeltaTime);	

	/** Get the correct stored mantle asset based on the type of mantle requested*/
	const FMantleAsset& GetMantleAsset(const EMantleType Type);

	/** Per frame update for mantle */
	void MantleTimelineUpdate(float Delta);

	/** When the mantle as finished */
	void MantleTimelineFinished();

	/** Sets the OwningCharacter locatin during ragdoll*/
	void SetActorLocationDuringRagdoll(ACharacter* OwningCharacter);	

	/** utility to get owning outer as an ACharacter */
	ACharacter* GetOwningCharacter()
	{
		return Cast<ACharacter>(const_cast<ACharacter*>(CharacterOwner.Get(false)));
	}

	/** stored character */
	TWeakObjectPtr<ACharacter> CharacterOwner;
};