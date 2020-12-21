#pragma once

#include "CoreMinimal.h"

#define SGNAME_DEF(__Name__)	 const FName __Name__ = PREPROCESSOR_TO_STRING(__Name__)
#define SGNAME_DEF_GROUP(__Name__, __VALUE__) const FName __Name__ = PREPROCESSOR_TO_STRING(__VALUE__)

namespace SGName
{
    SGNAME_DEF(Mask_AimOffset);
    SGNAME_DEF(YawOffset);
    SGNAME_DEF(RotationAmount);

    namespace Curve
    {
        SGNAME_DEF(BasePose_N);
        SGNAME_DEF(BasePose_CLF);
        SGNAME_DEF(Layering_Spine_Add);
        SGNAME_DEF(Layering_Head_Add);
        SGNAME_DEF(Layering_Arm_L_Add);
        SGNAME_DEF(Layering_Arm_R_Add);

        SGNAME_DEF(Layering_Hand_L);
        SGNAME_DEF(Layering_Hand_R);
        SGNAME_DEF(Layering_Arm_L);
        SGNAME_DEF(Layering_Arm_L_LS);
        SGNAME_DEF(Layering_Arm_R);
        SGNAME_DEF(Layering_Arm_R_LS);

        SGNAME_DEF(FootLock_L);
        SGNAME_DEF(FootLock_R);
        SGNAME_DEF(Enable_HandIK_L);
        SGNAME_DEF(Enable_HandIK_R);
        SGNAME_DEF(Enable_FootIK_L);
        SGNAME_DEF(Enable_FootIK_R);
        SGNAME_DEF(Enable_Transition);
    };

    namespace Bone
    {
        SGNAME_DEF_GROUP(IK_Foot_L, ik_foot_l);
        SGNAME_DEF_GROUP(IK_Foot_R, ik_foot_r);
        SGNAME_DEF_GROUP(Foot_Target_L, VB foot_target_l);
        SGNAME_DEF_GROUP(Foot_Target_R, VB foot_target_r);
    };
};