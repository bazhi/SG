#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ECharacter.h"

#include "SGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
protected:

#pragma region Config
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "References")
    class ASGCharacter* Character = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "References")
    float DeltaTimeX = 0;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Infomation")
    FRotator AimingRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    FVector Velocity = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    FVector RelativeVelocityDirection = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    FVector Acceleration = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    FVector MovementInput = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    bool IsMoving = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    bool HasMovementInput = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    float Speed = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    float MovementInputAmount = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    float AimYawRate = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    float ZoomAmount = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EMovementState MovementState = EMovementState::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EMovementState PrevMovementState = EMovementState::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EMovementAction MovementAction = EMovementAction::LowMantle;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    ERotationMode RotationMode = ERotationMode::VelocityDirection;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EGait Gait = EGait::Walking;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EStance Stance = EStance::Standing;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EViewMode ViewMode = EViewMode::ThirdPerson;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Infomation")
    EOverlayState OverlayState = EOverlayState::Default;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    EGroundedEntryState GroundedEntryState = EGroundedEntryState::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    EMovementDirection MovementDirection = EMovementDirection::Forward;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    EHipsDirection TrackedHipsDirection = EHipsDirection::F;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    FVector RelativeAccelerationAmount = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    bool ShouldMove = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    bool RotateL = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    bool RotateR = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    bool Pivot = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float RotateRate = 1.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float RotationScale = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float DiagonalScaleAmount = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float WalkRunBlend = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float StandingPlayRate = 1.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float CrouchingPlayRate = 1.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float StrideBlend = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float FYaw = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float BYaw = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float LYaw = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    float RYaw = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    FSGVelocityBlend VelocityBlend;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Grounded")
    FSGLeanAmount LeanAmount;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph In Air")
    bool Jumped = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph In Air")
    float JumpPlayRate = 1.2f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph In Air")
    float FallSpeed = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph In Air")
    float LandPrediction = 1.0f;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    FRotator SmoothedAimingRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    FRotator SpineRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    FVector2D AimingAngle = FVector2D::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    FVector2D SmoothedAimingAngle = FVector2D::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    float AimSweepTime = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    float InputYawOffsetTime = 0.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    float ForwardYawTime = 0.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    float LeftYawTime = 0.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Aiming Values")
    float RightYawTime = 0.f;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Ragdoll")
    float FlailRate = 0.f;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    int OverlayOverrideState = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float EnableAimOffset = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float BasePoseN = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float BasePoseCLF = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmL = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmLAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmLLS = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmLMS = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmR = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmRAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmRLS = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float ArmRMS = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float HandL = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float HandR = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float Legs = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float LegsAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float Pelvis = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float PelvisAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float Spine = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float SpineAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float Head = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float HeadAdd = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float EnableHandIKL = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Layer Blending")
    float EnableHandIKR = 0;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    float FootLockLAlpha = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    float FootLockRAlpha = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FVector FootLockLLocation = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FVector FootLockRLocation = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FRotator FootLockLRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FRotator FootLockRRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FVector FootOffsetLLocation = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FVector FootOffsetRLocation = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FRotator FootOffsetLRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FRotator FootOffsetRRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    FVector PelvisOffset = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimGraph Foot Ik")
    float PelvisAlpha = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float TurnCheckMinAngle = 45;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float Turn180Threshold = 130;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float AimYawRateLimit = 50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float ElapsedDelayTime = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float MinAngleDelay = 0.75f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    float MaxAngleDelay = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset NTurnIPL90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset NTurnIPR90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset NTurnIPL180;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset NTurnIPR180;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset CLFTurnIPL90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset CLFTurnIPR90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset CLFTurnIPL180;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn In Place")
    FSGTurnInPlaceAsset CLFTurnIPR180;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float RotateMinThreshold = -50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float RotateMaxThreshold = 50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float AimYawRateMinRange = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float AimYawRateMaxRange = 270;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float MinPlayRate = 1.15;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotate In Place")
    float MaxPlayRate = 3.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* DiagonalScaleAmountCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* StrideBlendNWalk = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* StrideBlendNRun = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* StrideBlendCWalk = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* LandPredictionCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* LeanInAirCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* YawOffsetFB = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blend Curves")
    UCurveFloat* YawOffsetLR = nullptr;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float AnimatedWalkSpeed = 150;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float AnimatedRunSpeed = 350;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float AnimatedSprintSpeed = 600;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float AnimatedCrouchSpeed = 150;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float VelocityBlendInterpSpeed = 12;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float GroundedLeanInterpSpeed = 4;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float InAirLeanInterpSpeed = 4;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float SmoothedAimingRotationInterpSpeed = 10;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float InputYawOffsetInterpSpeed = 8;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float TriggerPivotSpeedLimit = 200;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float FootHeight = 13.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float IKTraceDistanceAboveFoot = 50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float IKTraceDistanceBelowFoot = 45;
#pragma endregion

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    void UpdateCharacterInfo();

    float GetAnimCurveCompact(const FName& CurveName);
    float GetAnimCurveClamped(const FName& CurveName, float Bias, float ClampMin, float ClampMax);
};
