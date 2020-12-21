#pragma once

#include "CoreMinimal.h"
#include "SG/Core/SGStruct.h"
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
