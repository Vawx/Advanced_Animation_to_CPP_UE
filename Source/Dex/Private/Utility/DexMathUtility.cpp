#include "Utility/DexMathUtility.h"
#include "Math/DualQuat.h"

// Delta Rot
const FRotator DexMathUtility::NormalizedRotators(const FRotator& A, const FRotator& B)
{
	const FRotator Delta = A - B;
	const FRotator Result = Delta.GetNormalized();
	return Result;
}

// MapRangeClamped
const float DexMathUtility::RangeClamped(const float Value, const float A, const float B, const float C, const float D)
{
	const FVector2D RangeA = FVector2D(A, B);
	const FVector2D RangeB = FVector2D(C, D);
	const float ClampedPercent = FMath::Clamp<float>(FMath::GetRangePct(RangeA, Value), 0.f, 1.f);
	const float Result = FMath::GetRangeValue(RangeB, ClampedPercent);
	return Result;
}

const FVector DexMathUtility::VectorClampSizeMax(const FVector& A, const float Max)
{
	const FVector Result = A.GetClampedToMaxSize(Max);
	return Result;
}

// Shorthand for UKismetMathLibrary::InRange_FloatFloat
const bool DexMathUtility::InRangeFloatFloat(float Value, float Min, float Max)
{
	const bool Result = Value >= Min && Value <= Max;
	return Result;
}

bool DexMathUtility::IsByteDifferent(int A, int B)
{
	return A != B;
}

bool DexMathUtility::MultiTraceFoundHit(TArray<FHitResult> Hits)
{
	return Hits.Num() > 0;
}

FComponentAndTransform DexMathUtility::ComponentWorldToLocal(FComponentAndTransform WorldSpace)
{
	FComponentAndTransform Result = {};
	const UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(WorldSpace.Component.Get(false));
	if (Comp)
	{
		FTransform CompTransform = Comp->GetComponentToWorld();
		// Scale fucks with inverse and i would like to scale things sometimes
		CompTransform.SetScale3D(FVector::OneVector);
		
		FTransform Inverse = CompTransform.Inverse();
		Result.Transform = WorldSpace.Transform * Inverse;
		Result.Component = MakeWeakObjectPtr<UPrimitiveComponent>(WorldSpace.Component.Get(false));
	}
	return Result;
}

FComponentAndTransform DexMathUtility::ComponentLocalToWorld(FComponentAndTransform LocalSpace)
{
	FComponentAndTransform Result = {};
	const UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(LocalSpace.Component.Get(false));
	if (Comp)
	{
		FTransform CompTransform = Comp->GetComponentToWorld();
		// Scale fucks with inverse and i would like to scale things sometimes
		CompTransform.SetScale3D(FVector::OneVector);
		
		Result.Transform = LocalSpace.Transform * CompTransform;
		Result.Component = MakeWeakObjectPtr<UPrimitiveComponent>(LocalSpace.Component.Get(false));
	}
	return Result;
}

FTransform DexMathUtility::TransformMinusTransform(FTransform A, FTransform B)
{
	FTransform Result = FTransform::Identity;
	FVector ALoc = A.GetLocation();
	FVector BLoc = B.GetLocation();
	FVector RLoc = ALoc - BLoc;
	
	float AX = A.GetRotation().Rotator().Roll;
	float AY = A.GetRotation().Rotator().Pitch;
	float AZ = A.GetRotation().Rotator().Yaw;
	
	float BX = B.GetRotation().Rotator().Roll;
	float BY = B.GetRotation().Rotator().Pitch;
	float BZ = B.GetRotation().Rotator().Yaw;
	
	float Pitch = AY - BY;
	float Yaw = AZ - BZ;
	float Roll = AX - BX;
	
	FRotator Rot = FRotator(Pitch, Yaw, Roll);
	FVector Scale = A.GetScale3D() - B.GetScale3D();
	
	Result.SetLocation(RLoc);
	Result.SetRotation(Rot.Quaternion());
	Result.SetScale3D(Scale);
	
	return Result;
}

FTransform DexMathUtility::TransformPlusTransform(FTransform A, FTransform B)
{
	FTransform Result = FTransform::Identity;
	FVector ALoc = A.GetLocation();
	FVector BLoc = B.GetLocation();
	FVector RLoc = ALoc - BLoc;
	
	float AX = A.GetRotation().Rotator().Roll;
	float AY = A.GetRotation().Rotator().Pitch;
	float AZ = A.GetRotation().Rotator().Yaw;
	
	float BX = B.GetRotation().Rotator().Roll;
	float BY = B.GetRotation().Rotator().Pitch;
	float BZ = B.GetRotation().Rotator().Yaw;
	
	float Pitch = AY + BY;
	float Yaw = AZ + BZ;
	float Roll = AX + BX;
	
	FRotator Rot = FRotator(Pitch, Yaw, Roll);
	FVector Scale = A.GetScale3D() + B.GetScale3D();
	
	Result.SetLocation(RLoc);
	Result.SetRotation(Rot.Quaternion());
	Result.SetScale3D(Scale);
	
	return Result;
}

FTransform DexMathUtility::TransformLerp(const FTransform& A, const FTransform& B, float Alpha, EDexLerpInterpolationMode LerpInterpolationMode)
{
	FTransform Result;
	
	FTransform NA = A;
	FTransform NB = B;
	NA.NormalizeRotation();
	NB.NormalizeRotation();
	
	// Quaternion interpolation
	if (LerpInterpolationMode == EDexLerpInterpolationMode::QuatInterp)
	{
		Result.Blend(NA, NB, Alpha);
		return Result;
	}
	// Euler Angle interpolation
	else if (LerpInterpolationMode == EDexLerpInterpolationMode::EulerInterp)
	{
		Result.SetTranslation(FMath::Lerp(NA.GetTranslation(), NB.GetTranslation(), Alpha));
		Result.SetScale3D(FMath::Lerp(NA.GetScale3D(), NB.GetScale3D(), Alpha));
		Result.SetRotation(FQuat(RotatorLerp(NA.Rotator(), NB.Rotator(), Alpha, false)));
		return Result;
	}
	// Dual quaternion interpolation
	else
	{
		if ((NB.GetRotation() | NA.GetRotation()) < 0.0f)
		{
			NB.SetRotation(NB.GetRotation()*-1.0f);
		}
		return (FDualQuat(NA)*(1 - Alpha) + FDualQuat(NB)*Alpha).Normalized().AsFTransform(FMath::Lerp(NA.GetScale3D(), NB.GetScale3D(), Alpha));
	}
}

FRotator DexMathUtility::RotatorLerp(FRotator A, FRotator B, float Alpha, bool bShortestPath)
{
	// if shortest path, we use Quaternion to interpolate instead of using FRotator
	if (bShortestPath)
	{
		FQuat AQuat(A);
		FQuat BQuat(B);
		
		FQuat Result = FQuat::Slerp(AQuat, BQuat, Alpha);
		
		return Result.Rotator();
	}
	
	const FRotator DeltaAngle = B - A;
	return A + Alpha*DeltaAngle;
}