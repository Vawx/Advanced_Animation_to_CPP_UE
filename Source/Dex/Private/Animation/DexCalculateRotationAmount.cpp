

#include "Animation/DexCalculateRotationAmount.h"
#include "AnimationBlueprintLibrary.h"

void UDexCalculateRotationAmount::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (AnimationSequence)
	{
		if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, CurveName, ERawCurveTrackTypes::RCT_Float))
		{
			UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, CurveName, false);
		} 
		else
		{
			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, CurveName, ERawCurveTrackTypes::RCT_Float, false);
		}

		for (int32 AnimationFrame = 0; AnimationFrame < AnimationSequence->GetNumberOfSampledKeys(); ++AnimationFrame)
		{
			float FrameTime = 0.f;
			UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, AnimationFrame, FrameTime);

			float RateScale = 0;
			UAnimationBlueprintLibrary::GetRateScale(AnimationSequence, RateScale);

			int32 AnimationFrameOffset = AnimationFrame + ((RateScale < 0.f) ? -1 : 1);
			FTransform PoseA = FTransform::Identity;
			FTransform PoseB = FTransform::Identity;
			
			UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimationSequence, RootBoneName, AnimationFrameOffset, false, PoseA);
			UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimationSequence, RootBoneName, AnimationFrame, false, PoseB);			

			float Value = FMath::Abs(RateScale) * (PoseA.GetRotation().Z - PoseB.GetRotation().Z);
			UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, CurveName, FrameTime, Value);
		}
	}
}
