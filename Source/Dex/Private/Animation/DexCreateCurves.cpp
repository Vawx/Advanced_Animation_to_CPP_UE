

#include "Animation/DexCreateCurves.h"
#include "AnimationBlueprintLibrary.h"

UDexCreateCurves::UDexCreateCurves()
{

}

void UDexCreateCurves::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (AnimationSequence)
	{
		for (const FAnimCurveCreationParams CreationParams : CurvesToCreate)
		{
			if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, CreationParams.CurveName, ERawCurveTrackTypes::RCT_Float))
			{
				UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, CreationParams.CurveName, false);
			}
			else
			{
				UAnimationBlueprintLibrary::AddCurve(AnimationSequence, CreationParams.CurveName, ERawCurveTrackTypes::RCT_Float, false);
			}

			if (CreationParams.bKeyEachFrame)
			{
				for (int32 FrameIndex = 0; FrameIndex < AnimationSequence->GetNumberOfSampledKeys() - 2; ++FrameIndex)
				{
					float TimeAtFrame = 0.f;
					UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, FrameIndex, TimeAtFrame);
					UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, CreationParams.CurveName, TimeAtFrame, 0.f);
				}
			}
			else
			{
				for (const FAnimCurveCreationData& CreationData : CreationParams.Keys)
				{
					float CreationDataTime = 0.f;
					UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, CreationData.FrameNumber, CreationDataTime);
					UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, CreationParams.CurveName, CreationDataTime, CreationData.CurveValue);
				}
			}
		}
	}
}