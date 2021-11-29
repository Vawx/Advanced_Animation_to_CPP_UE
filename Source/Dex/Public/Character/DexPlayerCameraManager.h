#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DexPlayerCameraManager.generated.h"

class ADexCharacterBase;

UCLASS()
class DEX_API ADexPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CameraManager")
	USkeletalMeshComponent* CameraBehavior;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraManager")
	FVector RootLocation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraManager")
	FTransform SmoothedPivotPoint;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraManager")
	FVector PivotLocation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraManager")
	FVector TargetCameraLocation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CameraManager")
	FRotator TargetCameraRotation;
	
	template<typename T>
	T* GetControlledPawn()
	{
		if (!ControlledPawn.IsValid(false, false))
		{
			if (const APlayerController* PlayerController = GetOwningPlayerController())
			{
				ControlledPawn = Cast<T>(PlayerController->GetPawn());
			}
		}
		return Cast<T>(const_cast<T*>(ControlledPawn.Get(false)));		
	}
	
protected:
	
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;	
	virtual void BeginPlay() override;
	
	const FVector CalculateAxisIndependentLag(const FVector& CurrentLocation, const FVector& TargetLocation, const FVector& LagSpeeds, const FRotator& CameraRotation, const float DeltaTime) const;
	const float GetCameraBehaviorParam(const FName& CurveName) const;
	
	UPROPERTY()
	TWeakObjectPtr<ADexCharacterBase> ControlledPawn;	
};
