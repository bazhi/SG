#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "SGStruct.generated.h"

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

    FSGLeanAmount()
    {
        
    }

    FSGLeanAmount(float InLR, float InFB)
    {
        LR = InLR;
        FB = InFB;
    }

public:
    UPROPERTY(EditDefaultsOnly)
    float LR = 0;
    UPROPERTY(EditDefaultsOnly)
    float FB = 0;
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
