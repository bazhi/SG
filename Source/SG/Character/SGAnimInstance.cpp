#include "SGAnimInstance.h"
#include "SGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SG/Subsystem/ConfigWorldSubsystem.h"
#include "KismetAnimationLibrary.h"
#include "SG/Core/SGName.h"

void USGAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    Character = Cast<ASGCharacter>(TryGetPawnOwner());
}

void USGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    DeltaTimeX = DeltaSeconds;
    if (DeltaTimeX > 0.0f && IsValid(Character))
    {
        UpdateCharacterInfo();
        UpdateAimingValues();
        UpdateLayerValues();
        UpdateFootIK();

        switch (MovementState)
        {
            case EMovementState::None:
                break;
            case EMovementState::Grounded:
            {
                bool bPreShouldMove = ShouldMove;
                ShouldMove = ShouldMoveCheck();
                if (ShouldMove && !bPreShouldMove)
                {
                    ElapsedDelayTime = 0;
                    RotateL = false;
                    RotateR = false;
                }
                if (ShouldMove)
                {
                    UpdateMovementValues();
                    UpdateRotationValues();
                }
                else
                {
                    if (CanRotateInPlace())
                    {
                        RotateInPlaceCheck();
                    }
                    else
                    {
                        RotateL = false;
                        RotateR = false;
                    }
                    if (CanTurnInPlace())
                    {
                        TurnInPlaceCheck();
                    }
                    else
                    {
                        ElapsedDelayTime = 0;
                    }
                    if (CanDynamicTransition())
                    {
                        DynamicTransitionCheck();
                    }
                }
            }
            break;
            case EMovementState::InAir:
                UpdateInAirValues();
                break;
            case EMovementState::Mantling:
                break;
            case EMovementState::Ragdoll:
                UpdateRagdollValues();
                break;
            default: ;
        }
    }
}

void USGAnimInstance::PlayDynamicTransition(float ReTriggerDelay, const FSGDynamicMontageParams& Params)
{
}

void USGAnimInstance::UpdateCharacterInfo()
{
    //Get Information from the Character via the Character Interface to use throughout the AnimBP and AnimGraph.
    Character->GetEssentialValues(Velocity, Acceleration, MovementInput, IsMoving, HasMovementInput, Speed, MovementInputAmount, AimingRotation, AimYawRate);
    EMovementMode MovementMode;
    Character->GetCurrentStates(MovementMode, MovementState, PrevMovementState, MovementAction, RotationMode, Gait, Stance, ViewMode, OverlayState);
}

void USGAnimInstance::UpdateAimingValues()
{
    //Interp the Aiming Rotation value to achieve smooth aiming rotation changes. Interpolating the rotation before calculating the angle ensures the value is not affected by changes in actor rotation, allowing slow aiming rotation changes with fast actor rotation changes.
    SmoothedAimingRotation = UKismetMathLibrary::RInterpTo(SmoothedAimingRotation, AimingRotation, DeltaTimeX, SmoothedAimingRotationInterpSpeed);

    //Calculate the Aiming angle and Smoothed Aiming Angle by getting the delta between the aiming rotation and the actor rotation.
    FRotator LocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimingRotation, Character->GetActorRotation());
    AimingAngle.X = LocalRotation.Yaw;
    AimingAngle.Y = LocalRotation.Pitch;
    LocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(SmoothedAimingRotation, Character->GetActorRotation());
    SmoothedAimingAngle.X = LocalRotation.Yaw;
    SmoothedAimingAngle.Y = LocalRotation.Pitch;

    //Clamp the Aiming Pitch Angle to a range of 1 to 0 for use in the vertical aim sweeps.
    //Use the Aiming Yaw Angle divided by the number of spine+pelvis bones to get the amount of spine rotation needed to remain facing the camera direction.
    switch (RotationMode)
    {
        case ERotationMode::LookingDirection:
        case ERotationMode::Aiming:
        {
            AimSweepTime = UKismetMathLibrary::MapRangeClamped(AimingAngle.Y, -90, 90, 1.0, 1);
            SpineRotation.Roll = 0;
            SpineRotation.Pitch = 0;
            SpineRotation.Yaw = AimingAngle.X * 0.25f;
        }
            break;
        default: ;
    }

    //Get the delta between the Movement Input rotation and Actor rotation and map it to a range of 0-1. This value is used in the aim offset behavior to make the character look toward the Movement Input.
    if (ERotationMode::VelocityDirection == RotationMode)
    {
        if (HasMovementInput)
        {
            LocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(MovementInput.ToOrientationRotator(), Character->GetActorRotation());
            float ClampedValue = UKismetMathLibrary::MapRangeClamped(LocalRotation.Yaw, -180, 180, 0, 1.0);
            InputYawOffsetTime = UKismetMathLibrary::FInterpTo(InputYawOffsetTime, ClampedValue, DeltaTimeX, InputYawOffsetInterpSpeed);
        }
    }
    //Separate the Aiming Yaw Angle into 3 separate Yaw Times. These 3 values are used in the Aim Offset behavior to improve the blending of the aim offset when rotating completely around the character. This allows you to keep the aiming responsive but still smoothly blend from left to right or right to left.
    LeftYawTime = UKismetMathLibrary::MapRangeClamped(FMath::Abs(SmoothedAimingAngle.X), 0, 180, 0.5, 0);
    RightYawTime = UKismetMathLibrary::MapRangeClamped(FMath::Abs(SmoothedAimingAngle.X), 0, 180, 0.5, 1);
    ForwardYawTime = UKismetMathLibrary::MapRangeClamped(SmoothedAimingAngle.X, -180, 180, 0, 1);
}

void USGAnimInstance::UpdateLayerValues()
{
    //Get the Aim Offset weight by getting the opposite of the Aim Offset Mask.
    EnableAimOffset = UKismetMathLibrary::Lerp(1.0, 0, GetAnimCurveCompact(SGName::Mask_AimOffset));
    BasePoseN = GetAnimCurveCompact(SGName::Curve::BasePose_N);
    BasePoseCLF = GetAnimCurveCompact(SGName::Curve::BasePose_CLF);
    SpineAdd = GetAnimCurveCompact(SGName::Curve::Layering_Spine_Add);
    HeadAdd = GetAnimCurveCompact(SGName::Curve::Layering_Head_Add);
    ArmLAdd = GetAnimCurveCompact(SGName::Curve::Layering_Arm_L_Add);
    ArmRAdd = GetAnimCurveCompact(SGName::Curve::Layering_Arm_R_Add);

    EnableHandIKL = UKismetMathLibrary::Lerp(0, GetAnimCurveCompact(SGName::Curve::Enable_HandIK_L), GetAnimCurveCompact(SGName::Curve::Layering_Arm_L));
    EnableHandIKR = UKismetMathLibrary::Lerp(0, GetAnimCurveCompact(SGName::Curve::Enable_HandIK_R), GetAnimCurveCompact(SGName::Curve::Layering_Arm_R));

    ArmLLS = GetAnimCurveCompact(SGName::Curve::Layering_Arm_L_LS);
    ArmRLS = GetAnimCurveCompact(SGName::Curve::Layering_Arm_R_LS);
    ArmLMS = 1.0f - FMath::Floor(ArmLLS);
    ArmRMS = 1.0f - FMath::Floor(ArmRLS);
}

void USGAnimInstance::UpdateMovementValues()
{
}

void USGAnimInstance::UpdateRotationValues()
{
}

void USGAnimInstance::UpdateInAirValues()
{
}

void USGAnimInstance::UpdateRagdollValues()
{
}

void USGAnimInstance::UpdateFootIK()
{
}

bool USGAnimInstance::ShouldMoveCheck() const
{
    return IsMoving && HasMovementInput || Speed > 150.0f;
}

bool USGAnimInstance::CanRotateInPlace() const
{
    return ERotationMode::Aiming == RotationMode || EViewMode::FirstPerson == ViewMode;
}

bool USGAnimInstance::CanTurnInPlace() const
{
    return ERotationMode::LookingDirection == RotationMode || EViewMode::ThirdPerson == ViewMode;
}

bool USGAnimInstance::CanDynamicTransition() const
{
    return FMath::IsNearlyEqual(GetCurveValue(SGName::Curve::Enable_Transition), 1.0f);
}

bool USGAnimInstance::CanOverlayTransition() const
{
    return EStance::Standing == Stance && !ShouldMove;
}

float USGAnimInstance::GetAnimCurveCompact(const FName& CurveName)
{
    return GetCurveValue(CurveName);
}

float USGAnimInstance::GetAnimCurveClamped(const FName& CurveName, float Bias, float ClampMin, float ClampMax)
{
    float Value = GetCurveValue(CurveName);
    return FMath::Clamp(Value + Bias, ClampMin, ClampMax);
}

EDrawDebugTrace::Type USGAnimInstance::GetTraceDebugType(EDrawDebugTrace::Type DebugType)
{
    return DebugType;
}

void USGAnimInstance::TurnInPlace(const FRotator& TargetRotaion, float PlayRateScale, float StartTime, bool OverrideCurrent)
{
    ETurnInPlace TargetTurn = ETurnInPlace::None;
    float TurnAngle = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotaion, Character->GetActorRotation()).Yaw;
    //Step 2: Choose Turn Asset based on the Turn Angle and Stance
    if(FMath::Abs(TurnAngle) < Turn180Threshold)
    {
        if(TurnAngle < 0)
        {
            switch (Stance) {
                case EStance::Standing:
                    TargetTurn = ETurnInPlace::NL90;
                    break;
                case EStance::Crouching:
                    TargetTurn = ETurnInPlace::CLFL90;
                    break;
                default: ;
            }
        }else
        {
            switch (Stance)
            {
                case EStance::Standing:
                    TargetTurn = ETurnInPlace::NR90;
                    break;
                case EStance::Crouching:
                    TargetTurn = ETurnInPlace::CLFR90;
                    break;
                default:;
            }
        }
    }else
    {
        if (TurnAngle < 0)
        {
            switch (Stance)
            {
                case EStance::Standing:
                    TargetTurn = ETurnInPlace::NL180;
                    break;
                case EStance::Crouching:
                    TargetTurn = ETurnInPlace::CLFL180;
                    break;
                default:;
            }
        }
        else
        {
            switch (Stance)
            {
                case EStance::Standing:
                    TargetTurn = ETurnInPlace::NR180;
                    break;
                case EStance::Crouching:
                    TargetTurn = ETurnInPlace::CLFR180;
                    break;
                default:;
            }
        }
    }

    FDTRowTurnInPlace Query;
    Query.TurnMode = TargetTurn;

    const FDTRowTurnInPlace* DTRowTurnInPlace = nullptr;
    if (UConfigWorldSubsystem* Subsystem = UConfigWorldSubsystem::Get(this))
    {
        DTRowTurnInPlace = Subsystem->GetDataTableRow(Query);
    }
    if(!DTRowTurnInPlace)
    {
        return;
    }
    if(!IsPlayingSlotAnimation(DTRowTurnInPlace->Animation, DTRowTurnInPlace->SlotName) || OverrideCurrent)
    {
       UAnimMontage* AnimMontage = PlaySlotAnimationAsDynamicMontage(DTRowTurnInPlace->Animation, DTRowTurnInPlace->SlotName, 0.2, 0.2, DTRowTurnInPlace->PlayRate * PlayRateScale, 1, 0, StartTime);
        if(DTRowTurnInPlace->ScaleTurnAngle)
        {
            RotationScale = (TurnAngle / DTRowTurnInPlace->AnimatedAngle) * DTRowTurnInPlace->PlayRate * PlayRateScale;
        }else
        {
            RotationScale = DTRowTurnInPlace->PlayRate * PlayRateScale;
        }
    }
}

void USGAnimInstance::TurnInPlaceCheck()
{
    if(FMath::Abs(AimingAngle.X) > TurnCheckMinAngle && AimYawRate < AimYawRateLimit)
    {
        ElapsedDelayTime += DeltaTimeX;
        if(ElapsedDelayTime > UKismetMathLibrary::MapRangeClamped(FMath::Abs(AimingAngle.X), TurnCheckMinAngle, 180, MinAngleDelay, MaxAngleDelay))
        {
            TurnInPlace(FRotator(0, AimingRotation.Yaw, 0), 1.0, 0, false);
        }
    }else
    {
        ElapsedDelayTime = 0.0f;
    }
}

void USGAnimInstance::RotateInPlaceCheck()
{
    RotateL = AimingAngle.X < RotateMinThreshold;
    RotateR = AimingAngle.X > RotateMaxThreshold;

    if(RotateL || RotateR)
    {
        RotateRate = UKismetMathLibrary::MapRangeClamped(AimYawRate, AimYawRateMinRange, AimYawRateMaxRange, MinPlayRate, MaxPlayRate);
    }
}

void USGAnimInstance::DynamicTransitionCheck()
{
    if(UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(GetOwningComponent(), SGName::Bone::IK_Foot_L, ERelativeTransformSpace::RTS_Component, SGName::Bone::Foot_Target_L, ERelativeTransformSpace::RTS_Component, false, 0, 0, 0, 0) > 8)
    {
        PlayDynamicTransition(0.1f, DynamicTransitionLeft);
    }

    if (UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(GetOwningComponent(), SGName::Bone::IK_Foot_R, ERelativeTransformSpace::RTS_Component, SGName::Bone::Foot_Target_R, ERelativeTransformSpace::RTS_Component, false, 0, 0, 0, 0) > 8)
    {
        PlayDynamicTransition(0.1f, DynamicTransitionRight);
    }
}

void USGAnimInstance::SetFootOffsets(const FName EnableFootIKCurve, const FName& IKFootBone, const FName& RootBone, FVector& CurrentLocationTarget, FVector& CurrentLocationOffset, FRotator& CurrentRotationOffset)
{
    if (GetCurveValue(EnableFootIKCurve) > 0.f)
    {
        //Step 1: Trace downward from the foot location to find the geometry. If the surface is walkable, save the Impact Location and Normal.
        FVector IKFootBoneLocation = GetOwningComponent()->GetSocketLocation(IKFootBone);
        FVector RootBoneLocation = GetOwningComponent()->GetSocketLocation(RootBone);
        FVector IKFootFloorLocation(IKFootBoneLocation.X, IKFootBoneLocation.Y, RootBoneLocation.Z);

        FHitResult HitResult;
        TArray<AActor*> ActorToIgnore;
        UKismetSystemLibrary::LineTraceSingle(this, IKFootFloorLocation + FVector(0, 0, IKTraceDistanceAboveFoot), IKFootFloorLocation - FVector(0, 0, IKTraceDistanceBelowFoot)
                                              , TraceTypeFootIK, false, ActorToIgnore, GetTraceDebugType(EDrawDebugTrace::ForOneFrame), HitResult, true, FColor::Red, FColor::Yellow, 5.0f);

        FRotator TargetRotationOffset = FRotator::ZeroRotator;
        if (auto CharacterMovement = Character->GetCharacterMovement())
        {
            if (CharacterMovement->IsWalkable(HitResult))
            {
                FVector ImpactPoint = HitResult.ImpactPoint;
                FVector ImpactNormal = HitResult.ImpactNormal;

                //Step 1.1: Find the difference in location from the Impact point and the expected (flat) floor location. These values are offset by the nomrmal multiplied by the foot height to get better behavior on angled surfaces.
                CurrentLocationTarget = ImpactPoint + ImpactNormal * FootHeight - (IKFootFloorLocation + FVector::UpVector * FootHeight);
                //Step 1.2: Calculate the Rotation offset by getting the Atan2 of the Impact Normal.
                float X = UKismetMathLibrary::DegAtan2(ImpactNormal.Y, ImpactNormal.Z);
                float Y = -UKismetMathLibrary::DegAtan2(ImpactNormal.X, ImpactNormal.Z);
                TargetRotationOffset = UKismetMathLibrary::MakeRotator(X, Y, 0);
            }
        }

        //Step 2: Interp the Current Location Offset to the new target value. Interpolate at different speeds based on whether the new target is above or below the current one.
        float InterpSpeed = 15.0f;
        if (CurrentLocationOffset.Z > CurrentLocationTarget.Z)
        {
            InterpSpeed = 30.0f;
        }
        CurrentLocationOffset = UKismetMathLibrary::VInterpTo(CurrentLocationOffset, CurrentLocationTarget, DeltaTimeX, InterpSpeed);

        //Step 3: Interp the Current Rotation Offset to the new target value.
        CurrentRotationOffset = UKismetMathLibrary::RInterpTo(CurrentRotationOffset, TargetRotationOffset, DeltaTimeX, 30.0f);
    }
    else
    {
        CurrentLocationOffset = FVector::ZeroVector;
        CurrentRotationOffset = FRotator::ZeroRotator;
    }
}

void USGAnimInstance::SetPelvisIKOffset(const FVector& FootOffsetLTarget, const FVector& FootOffsetRTarget)
{

    PelvisAlpha = (GetCurveValue(SGName::Curve::Enable_FootIK_L) + GetCurveValue(SGName::Curve::Enable_FootIK_L)) * 0.5f;
    if (PelvisAlpha > 0)
    {
        FVector PelvisTarget = FVector::ZeroVector;
        //Step 1: Set the new Pelvis Target to be the lowest Foot Offset
        if(FootOffsetLTarget.Z < FootOffsetRTarget.Z)
        {
            PelvisTarget = FootOffsetLTarget;
        }else
        {
            PelvisTarget = FootOffsetRTarget;
        }

        //Step 2: Interp the Current Pelvis Offset to the new target value. Interpolate at different speeds based on whether the new target is above or below the current one.
        float InterpSpeed = 15.0f;
        if(PelvisTarget.Z > PelvisOffset.Z)
        {
            InterpSpeed = 10.0f;
        }
        PelvisOffset = UKismetMathLibrary::VInterpTo(PelvisOffset, PelvisTarget, DeltaTimeX, InterpSpeed);
    }
    else
    {
        PelvisOffset = FVector::ZeroVector;
    }
}

void USGAnimInstance::SetFootLocking(const FName& EnableFootIKCurve, const FName& FootLockCurve, const FName& IKFootBone, float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation)
{
    if (GetCurveValue(EnableFootIKCurve) > 0)
    {
        float FootLockCurveValue = GetCurveValue(FootLockCurve);
        if (FootLockCurveValue >= 0.99f || FootLockCurveValue < CurrentFootLockAlpha)
        {
            CurrentFootLockAlpha = FootLockCurveValue;
        }
        if (CurrentFootLockAlpha >= 0.99f)
        {
            FTransform LocalTransform = GetOwningComponent()->GetSocketTransform(IKFootBone, ERelativeTransformSpace::RTS_Component);
            CurrentFootLockLocation = LocalTransform.GetLocation();
            CurrentFootLockRotation = LocalTransform.Rotator();
        }
        if (CurrentFootLockAlpha > 0)
        {
            SetFootLockOffsets(CurrentFootLockLocation, CurrentFootLockRotation);
        }
    }
}

void USGAnimInstance::SetFootLockOffsets(FVector& LocalLocation, FRotator& LocalRotation)
{
    FRotator RotationDifference = FRotator::ZeroRotator;
    FVector LocationDifference = FVector::ZeroVector;
    if (auto CharacterMovement = Character->GetCharacterMovement())
    {
        if (CharacterMovement->IsMovingOnGround())
        {
            RotationDifference = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation(), CharacterMovement->GetLastUpdateRotation());
        }
    }
    LocationDifference = GetOwningComponent()->GetComponentRotation().UnrotateVector(Velocity * GetWorld()->GetDeltaSeconds());

    LocalLocation = UKismetMathLibrary::RotateAngleAxis(LocalLocation - LocationDifference, RotationDifference.Yaw, FVector::DownVector);
    LocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(LocalRotation, RotationDifference);
}

FSGVelocityBlend USGAnimInstance::CalculateVelocityBlend()
{
    FVector Dir = Character->GetActorRotation().UnrotateVector(Velocity.GetSafeNormal(0.1));
    float Sum = FMath::Abs(Dir.X) + FMath::Abs(Dir.Y) + FMath::Abs(Dir.Z);
    FVector RelativeDirection = Dir / Sum;
    FSGVelocityBlend Result;
    Result.F = UKismetMathLibrary::Clamp(RelativeDirection.X, 0, 1.0f);
    Result.B = FMath::Abs(UKismetMathLibrary::Clamp(RelativeDirection.X, -1, 0));

    return Result;
}
