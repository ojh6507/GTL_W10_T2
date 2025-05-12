#include "AnimationRuntime.h"
#include "Animation/AnimTypes.h"

void FAnimationRuntime::BlendTwoPosesTogether(
    const FCompactPose& SourcePose1,
    const FCompactPose& SourcePose2,
    const FBlendedCurve& SourceCurve1,
    const FBlendedCurve& SourceCurve2,
    const float			WeightOfPose1,
    /*out*/ FCompactPose& ResultPose,
    /*out*/ FBlendedCurve& ResultCurve)
{
    /*UE::Anim::FStackAttributeContainer TempAttributes;

    FAnimationPoseData AnimationPoseData = { ResultPose, ResultCurve, TempAttributes };

    const FAnimationPoseData SourceOnePoseData(const_cast<FCompactPose&>(SourcePose1), const_cast<FBlendedCurve&>(SourceCurve1), TempAttributes);
    const FAnimationPoseData SourceTwoPosedata(const_cast<FCompactPose&>(SourcePose2), const_cast<FBlendedCurve&>(SourceCurve2), TempAttributes);

    BlendTwoPosesTogether(SourceOnePoseData, SourceTwoPosedata, WeightOfPose1, AnimationPoseData);*/
}

void FAnimationRuntime::BlendTwoPosesTogether(const FAnimationPoseData& SourcePoseOneData, const FAnimationPoseData& SourcePoseTwoData, const float WeightOfPoseOne, /*out*/ FAnimationPoseData& OutAnimationPoseData)
{
    //FCompactPose& OutPose = OutAnimationPoseData.GetPose();
    //FBlendedCurve& OutCurve = OutAnimationPoseData.GetCurve();
    //UE::Anim::FStackAttributeContainer& OutAttributes = OutAnimationPoseData.GetAttributes();

    //const float WeightOfPoseTwo = 1.f - WeightOfPoseOne;

    //BlendPose<ETransformBlendMode::Overwrite>(SourcePoseOneData.GetPose(), OutPose, WeightOfPoseOne);
    //BlendPose<ETransformBlendMode::Accumulate>(SourcePoseTwoData.GetPose(), OutPose, WeightOfPoseTwo);

    //// Ensure that all of the resulting rotations are normalized
    //OutPose.NormalizeRotations();

    //OutCurve.Lerp(SourcePoseOneData.GetCurve(), SourcePoseTwoData.GetCurve(), WeightOfPoseTwo);
    //UE::Anim::Attributes::BlendAttributes({ SourcePoseOneData.GetAttributes(), SourcePoseTwoData.GetAttributes() }, { WeightOfPoseOne, WeightOfPoseTwo }, { 0, 1 }, OutAttributes);
}
