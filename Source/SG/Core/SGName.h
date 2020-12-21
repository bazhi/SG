#pragma once

#include "CoreMinimal.h"

#define SGNAME_DEFINE(__Name__)	 const FName __Name__ = PREPROCESSOR_TO_STRING(__Name__)
#define SGNAME_DEFINE2(__Name__,__VALUE__) const FName __Name__ = PREPROCESSOR_TO_STRING(__VALUE__)

namespace SGName
{
    SGNAME_DEFINE(Mask_AimOffset);
    SGNAME_DEFINE(YawOffset);
    SGNAME_DEFINE(RotationAmount);

    namespace Curve
    {
        SGNAME_DEFINE(BasePose_N);
        SGNAME_DEFINE(BasePose_CLF);
        SGNAME_DEFINE(Layering_Spine_Add);
        SGNAME_DEFINE(Layering_Head_Add);
        SGNAME_DEFINE(Layering_Arm_L_Add);
        SGNAME_DEFINE(Layering_Arm_R_Add);

        SGNAME_DEFINE(Layering_Hand_L);
        SGNAME_DEFINE(Layering_Hand_R);
        SGNAME_DEFINE(Layering_Arm_L);
        SGNAME_DEFINE(Layering_Arm_L_LS);
        SGNAME_DEFINE(Layering_Arm_R);
        SGNAME_DEFINE(Layering_Arm_R_LS);

        SGNAME_DEFINE(FootLock_L);
        SGNAME_DEFINE(FootLock_R);
        SGNAME_DEFINE(Enable_HandIK_L);
        SGNAME_DEFINE(Enable_HandIK_R);
        SGNAME_DEFINE(Enable_FootIK_L);
        SGNAME_DEFINE(Enable_FootIK_R);
        SGNAME_DEFINE(Enable_Transition);
    };

    namespace Bone
    {
        SGNAME_DEFINE2(IK_Foot_L, ik_foot_l);
        SGNAME_DEFINE2(IK_Foot_R, ik_foot_r);
        SGNAME_DEFINE2(Foot_Target_L, VB foot_target_l);
        SGNAME_DEFINE2(Foot_Target_R, VB foot_target_r);
    };
};