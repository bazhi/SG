#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ECharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SG/Interface/SGAnimationInterface.h"
#include "SGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGAnimInstance : public UAnimInstance, public ISGAnimationInterface
{
    GENERATED_BODY()
protected:

#pragma region Config
    UPROPERTY(Transient, BlueprintReadOnly, Category = "References")
    class ASGCharacter* Character = nullptr;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|References")
    float DeltaTimeX = 0.010448;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category="Debug|Character Infomation")
    FRotator AimingRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    FVector Velocity = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    FVector RelativeVelocityDirection = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    FVector Acceleration = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    FVector MovementInput = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    bool bIsMoving = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    bool bHasMovementInput = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    float Speed = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    float MovementInputAmount = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    float AimYawRate = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    float ZoomAmount = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EMovementState MovementState = EMovementState::None;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EMovementState PrevMovementState = EMovementState::None;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EMovementAction MovementAction = EMovementAction::LowMantle;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    ERotationMode RotationMode = ERotationMode::VelocityDirection;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EGait Gait = EGait::Walking;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EStance Stance = EStance::Standing;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EViewMode ViewMode = EViewMode::ThirdPerson;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Character Infomation")
    EOverlayState OverlayState = EOverlayState::Default;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    EGroundedEntryState GroundedEntryState = EGroundedEntryState::None;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    EMovementDirection MovementDirection = EMovementDirection::Forward;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|AnimGraph Grounded")
    EHipsDirection TrackedHipsDirection = EHipsDirection::F;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    FVector RelativeAccelerationAmount = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    bool bShouldMove = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    bool bRotateL = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    bool bRotateR = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    bool bPivot = false;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float RotateRate = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float RotationScale = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float DiagonalScaleAmount = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float WalkRunBlend = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float StandingPlayRate = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float CrouchingPlayRate = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float StrideBlend = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float FYaw = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float BYaw = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float LYaw = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    float RYaw = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    FSGVelocityBlend VelocityBlend;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Grounded")
    FSGLeanAmount LeanAmount;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph In Air")
    bool bJumped = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|AnimGraph In Air")
    float JumpPlayRate = 1.2f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph In Air")
    float FallSpeed = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph In Air")
    float LandPrediction = 1.0f;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    FRotator SmoothedAimingRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    FRotator SpineRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    FVector2D AimingAngle = FVector2D::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    FVector2D SmoothedAimingAngle = FVector2D::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    float AimSweepTime = 0.5f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    float InputYawOffsetTime = 0.f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    float ForwardYawTime = 0.f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    float LeftYawTime = 0.f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Aiming Values")
    float RightYawTime = 0.f;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Ragdoll")
    float FlailRate = 0.f;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    EOverlayOverrideState OverlayOverrideState = EOverlayOverrideState::Default;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float EnableAimOffset = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float BasePoseN = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float BasePoseCLF = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmL = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmLAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmLLS = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmLMS = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmR = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmRAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmRLS = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float ArmRMS = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float HandL = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float HandR = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float Legs = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float LegsAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float Pelvis = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float PelvisAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float Spine = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float SpineAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float Head = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float HeadAdd = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float EnableHandIKL = 1.0f;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Layer Blending")
    float EnableHandIKR = 1.0f;


    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    float FootLockLAlpha = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    float FootLockRAlpha = 0;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FVector FootLockLLocation = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FVector FootLockRLocation = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FRotator FootLockLRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FRotator FootLockRRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FVector FootOffsetLLocation = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FVector FootOffsetRLocation = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FRotator FootOffsetLRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FRotator FootOffsetRRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    FVector PelvisOffset = FVector::ZeroVector;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|AnimGraph Foot Ik")
    float PelvisAlpha = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float TurnCheckMinAngle = 45;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float Turn180Threshold = 130;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float AimYawRateLimit = 50;
    UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float ElapsedDelayTime = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float MinAngleDelay = 0.75f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Turn In Place")
    float MaxAngleDelay = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float RotateMinThreshold = -50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float RotateMaxThreshold = 50;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float AimYawRateMinRange = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float AimYawRateMaxRange = 270;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float MinPlayRate = 1.15;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Rotate In Place")
    float MaxPlayRate = 3.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* DiagonalScaleAmountCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* StrideBlendNWalk = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* StrideBlendNRun = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* StrideBlendCWalk = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* LandPredictionCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveFloat* LeanInAirCurve = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveVector* YawOffsetFB = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Blend Curves")
    UCurveVector* YawOffsetLR = nullptr;


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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Grounded")
    FSGDynamicMontageParams DynamicTransitionLeft;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Grounded")
    FSGDynamicMontageParams DynamicTransitionRight;

    UPROPERTY(Transient)
    TArray<FSGDynamicMontageTask> MontageTasks;
protected:
    float JumpedDelay = 0.0f;

#pragma endregion

public:
    virtual void OnJumped() override;
    virtual void SetGroundedEntryState(EGroundedEntryState InGroundedEntryState) override;
    virtual void SetOverlayOverrideState(EOverlayOverrideState InOverlayOverrideState) override;

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    virtual bool HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) override;

protected:
    //EventGraph
    void PlayTransition(const FSGDynamicMontageParams& Params);
    void PlayDynamicTransition(float ReTriggerDelay, const FSGDynamicMontageParams& Params);
    void TickMontageTasks(float DeltaSeconds);
protected:
    void UpdateCharacterInfo();
    void UpdateAimingValues();
    void UpdateLayerValues();
    void UpdateMovementValues();
    void UpdateRotationValues();
    void UpdateInAirValues();
    void UpdateRagdollValues();
    void UpdateFootIK();

protected:
    //Grounded
    FORCEINLINE_DEBUGGABLE bool ShouldMoveCheck() const;
    FORCEINLINE_DEBUGGABLE bool CanRotateInPlace() const;
    FORCEINLINE_DEBUGGABLE bool CanTurnInPlace() const;
    FORCEINLINE_DEBUGGABLE bool CanDynamicTransition() const;
    FORCEINLINE_DEBUGGABLE bool CanOverlayTransition() const;
    void TurnInPlace(const FRotator& TargetRotaion, float PlayRateScale, float StartTime, bool OverrideCurrent);
    void TurnInPlaceCheck();
    void RotateInPlaceCheck();
    void DynamicTransitionCheck();

    //FootIK
    void SetFootOffsets(const FName EnableFootIKCurve, const FName& IKFootBone, const FName& RootBone, FVector& CurrentLocationTarget, FVector& CurrentLocationOffset, FRotator& CurrentRotationOffset);
    void SetPelvisIKOffset(const FVector& FootOffsetLTarget, const FVector& FootOffsetRTarget);
    void SetFootLocking(const FName& EnableFootIKCurve, const FName& FootLockCurve, const FName& IKFootBone, float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation);
    void SetFootLockOffsets(FVector& LocalLocation, FRotator& LocalRotation);
    void ResetIKOffsets();

    //Movement
    FSGVelocityBlend CalculateVelocityBlend();
    float CalculateDiagonalScaleAmount();
    FVector CalculateRelativeAccelerationAmount();
    float CalculateWalkRunBlend();
    float CalculateStrideBlend();
    float CalculateStandingPlayRate();
    float CalculateCrouchingPlayRate();

    //InAir
    float CalculateLandPrediction();
    FSGLeanAmount CalculateInAirLeanAmount();

    //Rotation
    EMovementDirection CalculateMovementDirection();
    EMovementDirection CalculateQuadrant(EMovementDirection Current, float FR, float FL, float BR, float BL, float Buffer, float Angle);
    bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool bIncrease);

    //Interpolation
    FSGVelocityBlend InterpVelocityBlend(FSGVelocityBlend Current, FSGVelocityBlend Target, float InterpSpeed, float DeltaTime);
    FSGLeanAmount InterpLeanAmount(FSGLeanAmount Current, FSGLeanAmount Target, float InterpSpeed, float DeltaTime);


    //Curve
    float GetAnimCurveCompact(const FName& CurveName);
    float GetAnimCurveClamped(const FName& CurveName, float Bias, float ClampMin, float ClampMax);

protected:
    EDrawDebugTrace::Type GetTraceDebugType(EDrawDebugTrace::Type DebugType);

    //Visibility
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|ETraceTypeQuery")
    TEnumAsByte<ETraceTypeQuery> TraceTypeFootIK;
};
