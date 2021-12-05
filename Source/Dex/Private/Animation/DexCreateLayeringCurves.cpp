

#include "Animation/DexCreateLayeringCurves.h"
#include "AnimationBlueprintLibrary.h"

UDexCreateLayeringCurves::UDexCreateLayeringCurves()
{

}

void UDexCreateLayeringCurves::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (AnimationSequence)
	{
		for (const FName& Curve : CurvesToCreate)
		{
			if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, Curve, ERawCurveTrackTypes::RCT_Float))
			{
				UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, Curve, false);
			}
			else
			{
				UAnimationBlueprintLibrary::AddCurve(AnimationSequence, Curve, ERawCurveTrackTypes::RCT_Float, false);
			}

			if (bKeyEachFrame)
			{
				for (int32 FrameIndex = 0; FrameIndex < AnimationSequence->GetNumberOfSampledKeys() - 2; ++FrameIndex)
				{
					float Time = 0.f;
					UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, FrameIndex, Time);
					UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, Curve, Time, DefaultValue);
				}
			}
		}
	}
}