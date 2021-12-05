

#include "Animation/DexCopyCurves.h"
#include "Animation/AnimSequence.h"

UDexCopyCurves::UDexCopyCurves()
{
	AllCurves = { "BasePose_CLF", "BasePose_N", "Enable_AimOffset", "Enable_FootIK_L", "Enable_FootIK_R", "Enable_HandIK_L", "Enable_HandIK_R", "Enable_SpineRotation", "Enable_Transition", "Feet_Crossing", "FootLock_L", "FootLock_R", "HipOrientation_Bias", "Layering_Arm_L", "Layering_Arm_L_Add", "Layering_Arm_L_LS", "Layering_Arm_R", "Layering_Arm_R_Add", "Layering_Arm_R_LS", "Layering_Hand_L", "Layering_Hand_R", "Layering_Head", "Layering_Head_Add", "Layering_Legs", "Layering_Pelvis", "Layering_Spine", "Layering_Spine_Add", "Mask_AimOffset", "Mask_FootstepSound", "Mask_LandPrediction", "Mask_Lean", "RotationAmount", "Feet_Position", "Weight_Falling", "Weight_Gait", "YawOffset" };
}

void UDexCopyCurves::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (AnimationSequence)
	{
		if (AnimSequence != nullptr)
		{
			OnApplyDelayed(AnimSequence);
		}
	}
}

void UDexCopyCurves::Copy(TArray<FName> Curves)
{
	if (ensure(AnimSequence))
	{
		for (const FName& Curve : Curves)
		{
			if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, Curve, ERawCurveTrackTypes::RCT_Float))
			{
				if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, Curve, ERawCurveTrackTypes::RCT_Float))
				{
					UAnimationBlueprintLibrary::RemoveCurve(AnimSequence, Curve);
				}
				else
				{
					UAnimationBlueprintLibrary::AddCurve(AnimSequence, Curve);
				}
			}

			TArray<float> Times;
			TArray<float> Values;
			UAnimationBlueprintLibrary::GetFloatKeys(AnimSequence, Curve, Times, Values);
			UAnimationBlueprintLibrary::AddFloatCurveKeys(AnimSequence, Curve, Times, Values);
		}
	}
}

void UDexCopyCurves::OnApplyDelayed(UObject* Obj)
{
	if (AnimSequence && Obj && Obj == AnimSequence)
	{
		if (bCopyAllCurves)
		{
			Copy(AllCurves);
		}
		else
		{
			Copy(CurvesToCopy);
		}
	}
}
