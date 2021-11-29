#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/DexMovementData.h"
#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPtr.h"
#include "DexTimelineNative.h"
#include "Character/DexCharacterMovementData.h"
#include "DexCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDexCharacter, Log, All);

class UCameraComponent;
class AController;
class UAnimMontage;
class UCapsuleComponent;

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

UCLASS()
class DEX_API ADexCharacterBase : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "DVCharacter|Input")
	FDexCharacterRuntimeInputData CharacterRuntimeInputData;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")		
	float ThirdPersonFOV;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")		
	float FirstPersonFOV;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")		
	float LookUpDownRate;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")		
	float LookLeftRightRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DVCharacter|Movement")
	bool bRightShoulder;

	UDexCharacterMovementData* GetMovementData()
	{
		return MovementData.Get(false);
	}

protected:	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PIC) override;

	// input
	void Forward(const float Amount);
	void Right(const float Amount);
	void Look(const float Amount);
	void Up(const float Amount);
	void PlayerInputMovement(bool bForward);
	void FixDiagonalGameplayValues(float& YOut, float& XOut);

	TWeakObjectPtr<UDexCharacterMovementData> MovementData;

public:
	

	
};
