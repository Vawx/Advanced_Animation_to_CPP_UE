#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EDexLerpInterpolationMode : uint8
{
	/** Shortest Path or Quaternion interpolation for the rotation. */
	QuatInterp,

	/** Rotor or Euler Angle interpolation. */
	EulerInterp,

	/** Dual quaternion interpolation, follows helix or screw-motion path between keyframes.   */
	DualQuatInterp
};

class DEX_API DexMathUtility
{
public:
	DexMathUtility() {};
	~DexMathUtility() {};

	static const FRotator NormalizedRotators(const FRotator& A, const FRotator& B);
	static const float RangeClamped(const float Value, const float A, const float B, const float C, const float D);
	static const FVector VectorClampSizeMax(const FVector& A, const float Max);
	static const bool InRangeFloatFloat(float Value, float Min, float Max);	
	static bool IsByteDifferent(int A, int B);
	static bool MultiTraceFoundHit(TArray<FHitResult> Hits);
	//static FComponentAndTransform ComponentWorldToLocal(FComponentAndTransform WorldSpace);
	//static FComponentAndTransform ComponentLocalToWorld(FComponentAndTransform LocalSpace);
	static FTransform TransformMinusTransform(FTransform A, FTransform B);
	static FTransform TransformPlusTransform(FTransform A, FTransform B);	
	static FTransform TransformLerp(const FTransform& A, const FTransform& B, float Alpha, EDexLerpInterpolationMode LerpInterpolationMode = EDexLerpInterpolationMode::EulerInterp);
	static FRotator RotatorLerp(FRotator A, FRotator B, float Alpha, bool bShortestPath);
	static const float AnimCurveClamped(const UAnimInstance* AnimInst, const FName& CurveName, const float Bias, const float Min, const float Max);

	
};
