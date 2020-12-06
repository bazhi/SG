#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

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
    UAnimSequenceBase* Animation = nullptr;
    UPROPERTY(EditDefaultsOnly)
    float AnimatedAngle = 0;
    UPROPERTY(EditDefaultsOnly)
    FName SlotName = "None";
    UPROPERTY(EditDefaultsOnly)
    float PlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FSGMovementSettings
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float WalkSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    float RunSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    float SprintSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    class UCurveVector* MovementCurve = nullptr;
    UPROPERTY(EditDefaultsOnly)
    class UCurveFloat* RotationRateCurve = nullptr;
};

USTRUCT(BlueprintType)
struct FSGMovementSettingsStance
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FSGMovementSettings Standing;
    UPROPERTY(EditDefaultsOnly)
    FSGMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FSGMovementSettingsState : FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FSGMovementSettingsStance VelocityDirection;
    UPROPERTY(EditDefaultsOnly)
    FSGMovementSettingsStance LookingDirection;
    UPROPERTY(EditDefaultsOnly)
    FSGMovementSettingsStance Aiming;
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
struct FGSCameraSettings
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
struct FGSCameraSettingsGait
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettings Walking;
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettings Running;
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettings Sprinting;
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FGSCameraSettingsState
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettingsGait VelocityDirection;
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettingsGait LookingDirection;
    UPROPERTY(EditDefaultsOnly)
    FGSCameraSettingsGait Aiming;
};

USTRUCT(BlueprintType)
struct FGSDynamicMontageParams
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
struct FGSMantleAsset
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
    float LowHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float LowPlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float LowStartPosition = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighPlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighStartPosition = 0;
};

USTRUCT(BlueprintType)
struct FGSMantleParams
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
    float layRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float StartPosition = 0;
};

USTRUCT(BlueprintType)
struct FGSMantleTraceSettings
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
struct FGSRotateInPlaceAsset
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

USTRUCT(BlueprintType)
struct FGSTurnInPlaceAsset
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
