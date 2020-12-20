#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "ECharacter.generated.h"

UENUM(BlueprintType)
enum class EGait : uint8
{
    Walking,
    Running,
    Sprinting,
};

UENUM(BlueprintType)
enum class EMovementAction : uint8
{
    None,
    LowMantle,
    HighMantle,
    Rolling,
    GettingUp,
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    None,
    Grounded,
    InAir,
    Mantling,
    Ragdoll,
};

UENUM(BlueprintType)
enum class EOverlayState : uint8
{
    Default,
    Masculine,
    Feminine,
    Injured,
    HandsTied,
    Rifle,
    Pistol1H,
    Pistol2H,
    Bow,
    Torch,
    Binoculars,
    Box,
    Barrel,
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
    VelocityDirection,
    LookingDirection,
    Aiming,
};

UENUM(BlueprintType)
enum class EStance : uint8
{
    Standing,
    Crouching,
};

UENUM(BlueprintType)
enum class EViewMode : uint8
{
    ThirdPerson,
    FirstPerson,
};


UENUM(BlueprintType)
enum class EAnimFeature : uint8
{
    StrideBlending,
    AdditiveLeaning,
    SprintImpulse,
};

UENUM(BlueprintType)
enum class EFootstepType : uint8
{
    Step,
    WalkRun,
    Jump,
    Land,
};

UENUM(BlueprintType)
enum class EGroundedEntryState : uint8
{
    None,
    Roll,
};

UENUM(BlueprintType)
enum class EHipsDirection : uint8
{
    F,
    B,
    RF,
    RB,
    LF,
    LB,
};

UENUM(BlueprintType)
enum class EMantleType : uint8
{
    HighMantle,
    LowMantle,
    FallingCatch,
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
    Forward,
    Right,
    Left,
    Backward,
};

UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
    None,
    NL90,
    NR90,
    NL180,
    NR180,
    CLFL90,
    CLFR90,
    CLFL180,
    CLFR180,
};


USTRUCT(BlueprintType)
struct FSGVelocityBlend
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float F = 0;
    UPROPERTY(EditDefaultsOnly)
    float B = 0;
    UPROPERTY(EditDefaultsOnly)
    float L = 0;
    UPROPERTY(EditDefaultsOnly)
    float R = 0;
};

USTRUCT(BlueprintType)
struct FSGLeanAmount
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float LR = 0;
    UPROPERTY(EditDefaultsOnly)
    float FB = 0;
};

USTRUCT(BlueprintType)
struct FSGTurnInPlaceAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    class UAnimSequenceBase* Animation = nullptr;
    UPROPERTY(EditDefaultsOnly)
    float AnimatedAngle = 0;
    UPROPERTY(EditDefaultsOnly)
    FName SlotName = "None";
    UPROPERTY(EditDefaultsOnly)
    float PlayRate = 1.0f;
    UPROPERTY(EditDefaultsOnly)
    bool ScaleTurnAngle = true;
};

USTRUCT(BlueprintType)
struct FGSComponentAndTransform
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FTransform Transform;
    UPROPERTY(EditDefaultsOnly)
    class UPrimitiveComponent* Component;
};

USTRUCT(BlueprintType)
struct FSGCameraSettings
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float TargetArmLength = 0;
    UPROPERTY(EditDefaultsOnly)
    FVector SocketOffset = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly)
    float LagSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    float RotationLagSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    bool DoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FSGCameraSettingsGait
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettings Walking;
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettings Running;
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettings Sprinting;
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FSGCameraSettingsState
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettingsGait VelocityDirection;
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettingsGait LookingDirection;
    UPROPERTY(EditDefaultsOnly)
    FSGCameraSettingsGait Aiming;
};

USTRUCT(BlueprintType)
struct FSGDynamicMontageParams
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    UAnimSequenceBase* Animation = nullptr;
    UPROPERTY(EditDefaultsOnly)
    float BlendInTime = 0;
    UPROPERTY(EditDefaultsOnly)
    float BlendOutTime = 0;
    UPROPERTY(EditDefaultsOnly)
    float PlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float StartTime = 0;
};

USTRUCT(BlueprintType)
struct FSGMantleParams
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    class UAnimMontage* AnimMontage = nullptr;
    UPROPERTY(EditDefaultsOnly)
    class UCurveVector* PositionCorrectionCurve = nullptr;
    UPROPERTY(EditDefaultsOnly)
    FVector StartingOffset = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly)
    float PlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float StartingPosition = 0;
};

USTRUCT(BlueprintType)
struct FSGMantleTraceSettings
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float MaxLedgeHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float MinLedgeHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float ReachDistance = 0;
    UPROPERTY(EditDefaultsOnly)
    float ForwardTraceRadius = 0;
    UPROPERTY(EditDefaultsOnly)
    float DownwardTraceRadius = 0;
};

USTRUCT(BlueprintType)
struct FSGRotateInPlaceAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    class UAnimSequenceBase* Animation = nullptr;
    UPROPERTY(EditDefaultsOnly)
    FName SlotName = "None";
    UPROPERTY(EditDefaultsOnly)
    float SlowTurnRate = 90;
    UPROPERTY(EditDefaultsOnly)
    float FastTurnRate = 90;
    UPROPERTY(EditDefaultsOnly)
    float SlowPlayRate = 1.0f;
    UPROPERTY(EditDefaultsOnly)
    float FastPlayRate = 1.0f;
};

namespace SGName
{
    const FName Mask_AimOffset = "Mask_AimOffset";
    const FName YawOffset = "YawOffset";
    const FName RotationAmount = "RotationAmount";

    namespace Curve
    {
        const FName BasePose_N = "BasePose_N";
        const FName BasePose_CLF = "BasePose_CLF";
        const FName Layering_Spine_Add = "Layering_Spine_Add";
        const FName Layering_Head_Add = "Layering_Head_Add";
        const FName Layering_Arm_L_Add = "Layering_Arm_L_Add";
        const FName Layering_Arm_R_Add = "Layering_Arm_R_Add";

        const FName Layering_Hand_R = "Layering_Hand_R";
        const FName Layering_Hand_L = "Layering_Hand_L";
        const FName Enable_HandIK_L = "Enable_HandIK_L";
        const FName Layering_Arm_L = "Layering_Arm_L";
        const FName Enable_HandIK_R = "Enable_HandIK_R";
        const FName Layering_Arm_R = "Layering_Arm_R";
        const FName Layering_Arm_L_LS = "Layering_Arm_L_LS";
        const FName Layering_Arm_R_LS = "Layering_Arm_R_LS";

        const FName FootLock_L = "FootLock_L";
        const FName FootLock_R = "FootLock_R";
        const FName Enable_FootIK_L = "Enable_FootIK_L";
        const FName Enable_FootIK_R = "Enable_FootIK_R";

        const FName Enable_Transition = "Enable_Transition";
    };

    namespace Bone
    {
        const FName IK_Foot_L = "ik_foot_l";
        const FName IK_Foot_R = "ik_foot_r";
        const FName Foot_Target_L = "VB foot_target_l";
        const FName Foot_Target_R = "VB foot_target_r";

    }
};
