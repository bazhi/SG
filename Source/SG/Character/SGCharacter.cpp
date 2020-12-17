#include "SGCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GSInputName.h"
#include "Components/TimelineComponent.h"

#include "Curves/CurveVector.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"

#include "SG/BlueprintLibrary/GSBlueprintLibrary.h"
#include "SG/DataAsset/TableRowDefine.h"
#include "SG/Subsystem/ConfigGameSubsystem.h"

const FName Name_RagdollPose = "RagdollPose";
const FName Name_BoneRoot = "root";
const FName Name_BonePelvis = "pelvis";

// Sets default values
ASGCharacter::ASGCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
    Super::BeginPlay();
    OnBeginPlay();
}

void ASGCharacter::OnBeginPlay()
{
    if (auto MeshTemp = GetMesh())
    {
        MeshTemp->AddTickPrerequisiteActor(this);
        MainAnimInstance = MeshTemp->GetAnimInstance();
        SetMovementModel();
    }
    OnGaitChanged(DesiredGait);
    OnRotationModeChanged(DesiredRotationMode);
    OnViewModeChanged(ViewMode);
    OnOverlayStateChanged(OverlayState);
    switch (DesiredStance)
    {
        case EStance::Standing:
            UnCrouch();
            break;

        case EStance::Crouching:
            Crouch();
            break;
        default: ;
    }
    TargetRotation = GetActorRotation();
    LastVelocityRotation = TargetRotation;
    LastMovementInputRotation = TargetRotation;
}

void ASGCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
    OnStanceChanged(EStance::Crouching);
}

void ASGCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
    OnStanceChanged(EStance::Standing);
}

void ASGCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        OnCharacteMovementModeChanged(PrevMovementMode, LocalCharacterMovement->MovementMode, PreviousCustomMode, LocalCharacterMovement->CustomMovementMode);
    }
}

void ASGCharacter::OnJumped_Implementation()
{
    Super::OnJumped_Implementation();
    if (Speed > 100.0f)
    {
        InAirRotation = LastVelocityRotation;
    }
    else
    {
        InAirRotation = GetActorRotation();
    }
    if (MainAnimInstance)
    {
        //MainAnimInstance->Jum
    }
}

void ASGCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    if (BreakFall)
    {
        BreakFallEvent();
    }
    else if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        if (HasMovementInput)
        {
            LocalCharacterMovement->BrakingFrictionFactor = 0.5f;
        }
        else
        {
            LocalCharacterMovement->BrakingFrictionFactor = 3.0f;
        }
        GetWorldTimerManager().SetTimer(TimerResetBrakingFactor, this, &ASGCharacter::OnResetBrakingFrctionFactor, 0.5f, false);
    }
}

void ASGCharacter::BreakFallEvent()
{
    if (MainAnimInstance)
    {
        MainAnimInstance->Montage_Play(GetRollAnimation(), 1.35f, EMontagePlayReturnType::MontageLength, 0, true);
    }
}

void ASGCharacter::RollEvent()
{
    if (MainAnimInstance)
    {
        MainAnimInstance->Montage_Play(GetRollAnimation(), 1.15f, EMontagePlayReturnType::MontageLength, 0, true);
    }
}

void ASGCharacter::OnCharacteMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode, uint8 PrevCustomMode, uint8 NewCustomMode)
{
    switch (NewMovementMode)
    {
        case MOVE_Walking:
        case MOVE_NavWalking:
            SetMovementState(EMovementState::Grounded);
            break;
        case MOVE_Falling:
            SetMovementState(EMovementState::InAir);
            break;
        default: ;
    }
}

void ASGCharacter::OnMovementStateChanged(EMovementState NewMovementState)
{
    EMovementState PreviousMovementState = MovementState;
    MovementState = NewMovementState;

    switch (MovementState)
    {
        case EMovementState::InAir:
        {
            switch (MovementAction)
            {
                case EMovementAction::None:
                    InAirRotation = GetActorRotation();
                    switch (Stance)
                    {
                        case EStance::Standing:
                            break;
                        case EStance::Crouching:
                            UnCrouch();
                            break;
                        default: ;
                    }
                    break;
                case EMovementAction::Rolling:
                    RagdollStart();
                    break;
                default: ;
            }
        }
        break;
        case EMovementState::Mantling:
            break;
        case EMovementState::Ragdoll:
            switch (PreviousMovementState)
            {
                case EMovementState::Mantling:
                    if (MantleTimeline)
                    {
                        MantleTimeline->Stop();
                    }
                    break;
                default: ;
            }
            break;
        default: ;
    }
}

void ASGCharacter::OnMovementActionChanged(EMovementAction NewMovementAction)
{
    EMovementAction PreviousMovementAction = MovementAction;
    MovementAction = NewMovementAction;
    switch (MovementAction)
    {
        case EMovementAction::Rolling:
            Crouch();
            break;
        default: ;
    }
    switch (PreviousMovementAction)
    {
        case EMovementAction::Rolling:
            switch (DesiredStance)
            {
                case EStance::Standing:
                    UnCrouch();
                    break;
                case EStance::Crouching:
                    Crouch();
                    break;
                default: ;
            }
            break;
        default: ;
    }
}

void ASGCharacter::OnStanceChanged(EStance NewStance)
{
    EStance PreviousStance = Stance;
    Stance = NewStance;
}

void ASGCharacter::OnRotationModeChanged(ERotationMode NewRotationMode)
{
    ERotationMode PreviousRotationMode = RotationMode;
    RotationMode = NewRotationMode;

    switch (RotationMode)
    {
        case ERotationMode::VelocityDirection:
            switch (ViewMode)
            {
                case EViewMode::FirstPerson:
                    SetViewMode(EViewMode::ThirdPerson);
                    break;
                default: ;
            }
            break;
        default: ;
    }
}

void ASGCharacter::OnGaitChanged(EGait NewActualGait)
{
    EGait PreviousActualGait = Gait;
    Gait = NewActualGait;
}

void ASGCharacter::OnViewModeChanged(EViewMode NewViewMode)
{
    EViewMode PreviousViewMode = ViewMode;
    ViewMode = NewViewMode;
    switch (ViewMode)
    {
        case EViewMode::ThirdPerson:
            switch (RotationMode)
            {
                case ERotationMode::VelocityDirection:
                case ERotationMode::LookingDirection:
                    SetRotationMode(DesiredRotationMode);
                    break;
                default: ;
            }
            break;
        case EViewMode::FirstPerson:
            switch (RotationMode)
            {
                case ERotationMode::VelocityDirection:
                    SetRotationMode(ERotationMode::LookingDirection);
                    break;
                default: ;
            }
            break;
        default: ;
    }
}

void ASGCharacter::OnOverlayStateChanged(EOverlayState NewOverlayState)
{
    EOverlayState PreviousOverlayState = OverlayState;
    OverlayState = NewOverlayState;

    FDTRowOverlayState Query;
    Query.ID = OverlayState;

    if (UConfigWorldSubsystem* Subsystem = UConfigWorldSubsystem::Get(this))
    {
        DTRowOverlayState = Subsystem->GetDataTableRow(Query);
    }
}


void ASGCharacter::SetMovementModel()
{
    if (MovementModel.DataTable)
    {
        if (auto FindResult = MovementModel.DataTable->FindRow<FSGMovementSettingsState>(MovementModel.RowName, ""))
        {
            MovementData = *FindResult;
        }
    }
}

void ASGCharacter::UpdateCharacterMovement()
{
    EGait AllowedGait = GetAllowedGait();
    EGait ActualGait = GetActualGait(AllowedGait);
    if (ActualGait != Gait)
    {
        SetGait(ActualGait);
        UpdateDynamicMovementSettings(AllowedGait);
    }
}

void ASGCharacter::UpdateDynamicMovementSettings(EGait AllowedGait)
{
    CurrentMovementSettings = GetTargetMovementSettings();
    float AllowedSpeed = 0;
    switch (AllowedGait)
    {
        case EGait::Walking:
            AllowedSpeed = CurrentMovementSettings.WalkSpeed;
            break;
        case EGait::Running:
            AllowedSpeed = CurrentMovementSettings.RunSpeed;
            break;
        case EGait::Sprinting:
            AllowedSpeed = CurrentMovementSettings.SprintSpeed;
            break;
        default: ;
    }
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        LocalCharacterMovement->MaxWalkSpeed = AllowedSpeed;
        LocalCharacterMovement->MaxWalkSpeedCrouched = AllowedSpeed;

        float InTime = GetMappedSpeed();
        if (CurrentMovementSettings.MovementCurve)
        {
            FVector VectorValue = CurrentMovementSettings.MovementCurve->GetVectorValue(InTime);
            LocalCharacterMovement->MaxAcceleration = VectorValue.X;
            LocalCharacterMovement->BrakingDecelerationWalking = VectorValue.Y;
            LocalCharacterMovement->GroundFriction = VectorValue.Z;
        }
    }
}

FSGMovementSettings ASGCharacter::GetTargetMovementSettings()
{
    const FSGMovementSettingsStance* SettingsStance = nullptr;
    switch (RotationMode)
    {
        case ERotationMode::VelocityDirection:
            SettingsStance = &MovementData.VelocityDirection;
            break;
        case ERotationMode::LookingDirection:
            SettingsStance = &MovementData.LookingDirection;
            break;
        case ERotationMode::Aiming:
            SettingsStance = &MovementData.Aiming;
            break;
        default: ;
    }
    if (SettingsStance)
    {
        switch (Stance)
        {
            case EStance::Standing:
                return SettingsStance->Standing;
            case EStance::Crouching:
                return SettingsStance->Crouching;
            default: ;
        }
    }
    return FSGMovementSettings();
}

float ASGCharacter::GetMappedSpeed()
{
    float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
    float LocRunSpeed = CurrentMovementSettings.RunSpeed;
    float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;
    if (Speed <= LocWalkSpeed)
    {
        return UKismetMathLibrary::MapRangeClamped(Speed, 0.f, LocWalkSpeed, 0.f, 1.f);
    }
    else if (Speed <= LocRunSpeed)
    {
        return UKismetMathLibrary::MapRangeClamped(Speed, LocWalkSpeed, LocRunSpeed, 1.f, 2.f);
    }
    else
    {
        return UKismetMathLibrary::MapRangeClamped(Speed, LocRunSpeed, LocSprintSpeed, 2.f, 3.f);
    }
}

EGait ASGCharacter::GetAllowedGait()
{
    if (EStance::Standing == Stance && (ERotationMode::VelocityDirection == RotationMode || ERotationMode::LookingDirection == RotationMode))
    {
        if (EGait::Sprinting == DesiredGait)
        {
            if (CanSprint())
            {
                return EGait::Sprinting;
            }
            return EGait::Running;
        }
        return DesiredGait;
    }
    else
    {
        if (EGait::Walking == DesiredGait)
        {
            return EGait::Walking;
        }
        else
        {
            return EGait::Running;
        }
    }
}

EGait ASGCharacter::GetActualGait(EGait AllowedGait)
{
    float LocalWalkSpeed = CurrentMovementSettings.WalkSpeed;
    float LocalRunSpeed = CurrentMovementSettings.RunSpeed;
    //float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;
    if (Speed >= LocalRunSpeed + 10.0f)
    {
        if (EGait::Sprinting == AllowedGait)
        {
            return EGait::Sprinting;
        }
        else
        {
            return EGait::Running;
        }
    }
    else if (Speed >= LocalWalkSpeed + 10.0f)
    {
        return EGait::Running;
    }
    else
    {
        return EGait::Walking;
    }
}

bool ASGCharacter::CanSprint()
{
    if (HasMovementInput)
    {
        switch (RotationMode)
        {
            case ERotationMode::VelocityDirection:
                return MovementInputAmount > 0.9f;
                break;
            case ERotationMode::LookingDirection:
                if (auto CharMovement = GetCharacterMovement())
                {
                    FRotator OrientationRotator = CharMovement->GetCurrentAcceleration().ToOrientationRotator();
                    FRotator ControlRotation = GetControlRotation();
                    FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(OrientationRotator, ControlRotation);
                    return MovementInputAmount > 0.9f && FMath::Abs(DeltaRotator.Yaw) < 50.0f;
                }
                break;
            case ERotationMode::Aiming:
                return false;
                break;
            default: ;
        }
    }
    return false;
}

UAnimMontage* ASGCharacter::GetRollAnimation() const
{
    if(DTRowOverlayState)
    {
        if (UConfigWorldSubsystem* Subsystem = UConfigWorldSubsystem::Get(this))
        {
            return Subsystem->GetCacheable(DTRowOverlayState->AnimMontageRoll);
        }
    }
    return nullptr;
}

void ASGCharacter::UpdateGroundedRotation()
{
    if (EMovementAction::Rolling == MovementAction)
    {
        if (HasMovementInput)
        {
            SmoothCharacterRotation(FRotator(0, 0, LastMovementInputRotation.Yaw), 0.f, 2.f);
        }
    }
    else if (EMovementAction::None == MovementAction)
    {
        if (CanUpdateMovingRotation())
        {
            switch (RotationMode)
            {
                case ERotationMode::VelocityDirection:
                    SmoothCharacterRotation(FRotator(0, 0, LastVelocityRotation.Yaw), 800, CalculateGroundedRotationRate());
                    break;
                case ERotationMode::LookingDirection:
                    switch (Gait)
                    {
                        case EGait::Walking:
                        case EGait::Running:
                            SmoothCharacterRotation(FRotator(0, 0, GetControlRotation().Yaw + GetAnimCurveValue("YawOffset")), 500, CalculateGroundedRotationRate());
                            break;
                        case EGait::Sprinting:
                            SmoothCharacterRotation(FRotator(0, 0, GetControlRotation().Yaw), 1000, 20);
                            break;
                        default: ;
                    }
                    break;
                case ERotationMode::Aiming:
                    break;
                default: ;
            }
        }
        else
        {
            if (EViewMode::FirstPerson == ViewMode || ERotationMode::Aiming == RotationMode)
            {
                LimitRotation(-100, 100, 20);
            }

            float RotationAmount = GetAnimCurveValue("RotationAmount");
            if (FMath::Abs(RotationAmount) > 0.001f)
            {
                AddActorWorldRotation(FRotator(0, 0, GetWorld()->GetDeltaSeconds() * 30.0f * RotationAmount));
                TargetRotation = GetActorRotation();
            }
        }
    }
}

void ASGCharacter::UpdateInAirRotation()
{
    switch (RotationMode)
    {
        case ERotationMode::VelocityDirection:
        case ERotationMode::LookingDirection:
            SmoothCharacterRotation(FRotator(0, 0, InAirRotation.Yaw), 0, 5);
            break;
        case ERotationMode::Aiming:
            SmoothCharacterRotation(FRotator(0, 0, GetControlRotation().Yaw), 0, 15.0f);
            break;
        default: ;
    }
}

void ASGCharacter::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed)
{
    float Delta = GetWorld()->GetDeltaSeconds();
    TargetRotation = UKismetMathLibrary::RInterpTo_Constant(TargetRotation, Target, Delta, TargetInterpSpeed);
    SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, Delta, ActorInterpSpeed));
}

void ASGCharacter::AddToCharacterRotation(FRotator DeltaRotation)
{
    TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, DeltaRotation);
    AddActorWorldRotation(DeltaRotation);
}

void ASGCharacter::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed)
{
    FRotator ControlRotation = GetControlRotation();
    FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, GetActorRotation());
    bool Condition = UKismetMathLibrary::InRange_FloatFloat(DeltaRotation.Yaw, AimYawMin, AimYawMax, true, true);
    float A = AimYawMin + ControlRotation.Yaw;
    float B = AimYawMax + ControlRotation.Yaw;
    float PickZ = DeltaRotation.Yaw > 0 ? A : B;
    SmoothCharacterRotation(FRotator(0, 0, PickZ), 0, InterpSpeed);
}

bool ASGCharacter::SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult& SweepHitResult)
{
    TargetRotation = NewRotation;
    return K2_SetActorLocationAndRotation(NewLocation, NewRotation, bSweep, SweepHitResult, bTeleport);
}

float ASGCharacter::CalculateGroundedRotationRate()
{
    if (CurrentMovementSettings.RotationRateCurve)
    {
        float CurveFloat = CurrentMovementSettings.RotationRateCurve->GetFloatValue(GetMappedSpeed());
        return CurveFloat * UKismetMathLibrary::MapRangeClamped(AimYawRate, 0, 300, 1, 3);
    }
    return 0.0f;
}

bool ASGCharacter::CanUpdateMovingRotation()
{
    return ((IsMoving && HasMovementInput) || (Speed > 150.0f)) && !HasAnyRootMotion();
}

bool ASGCharacter::MantleCheck(FGSMantleTraceSettings TraceSettings, EDrawDebugTrace::Type DebugType)
{
    DebugType = GetTraceDebugType(DebugType);
    FVector BaseLocation = GetCapsuleBaseLocation(2.0f);
    FVector PlayerMoveInput = GetPlayerMovementInput();
    float SumHeightAverage = (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) * 0.5f;
    float DiffHeightAverage = (TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) * 0.5f;
    FVector Start = BaseLocation + PlayerMoveInput * -30.0f + FVector(0, 0, SumHeightAverage);
    FVector End = Start + PlayerMoveInput * TraceSettings.ReachDistance;
    float Radius = TraceSettings.ForwardTraceRadius;
    float HalfHeight = DiffHeightAverage + 1.0f;
    TArray<AActor*> ActorsToIgnore;
    FHitResult HitResult;
    UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), Start, End, Radius, HalfHeight, TraceTypeClimbable, false, ActorsToIgnore, DebugType, HitResult, true, FColor::Yellow, FColor::Red, 1.0f);
    FVector InitialTraceImpactPoint;
    FVector InitialTraceNormal;
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        bool bWalkable = LocalCharacterMovement->IsWalkable(HitResult);

        if (!bWalkable && HitResult.bBlockingHit && !HitResult.bStartPenetrating)
        {
            InitialTraceImpactPoint = HitResult.ImpactPoint;
            InitialTraceNormal = HitResult.ImpactNormal;
        }
        else
        {
            return false;
        }
    }

    End = FVector(InitialTraceImpactPoint.X, InitialTraceImpactPoint.Y, GetCapsuleBaseLocation(2).Z) + InitialTraceNormal * -15;
    Start = End;
    Start.Z += (TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.0f);
    Radius = TraceSettings.DownwardTraceRadius;
    UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), Start, End, Radius, HalfHeight, TraceTypeClimbable, false, ActorsToIgnore, DebugType, HitResult, true, FColor::Yellow, FColor::Red, 1.0f);

    FVector DownTraceLocation;
    UPrimitiveComponent* HitComponent = nullptr;
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        bool bWalkable = LocalCharacterMovement->IsWalkable(HitResult);
        if (bWalkable && HitResult.bBlockingHit)
        {
            DownTraceLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
            HitComponent = HitResult.Component.Get();
        }
        else
        {
            return false;
        }
    }
    FVector TargetLocation = GetCapsuleLocationFromBase(DownTraceLocation, 2.0f);
    if (!CapsuleHasRoomCheck(GetCapsuleComponent(), TargetLocation, 0, 0, DebugType))
    {
        return false;
    }
    FRotator TargetTransformRotation = (InitialTraceNormal * FVector(-1, -1, 0)).ToOrientationRotator();
    FTransform TargetTransform = UKismetMathLibrary::MakeTransform(TargetLocation, TargetTransformRotation, FVector::OneVector);
    float MantleHeight = TargetTransform.GetLocation().Z - GetActorLocation().Z;

    EMantleType MantleType = EMantleType::LowMantle;
    switch (MovementState)
    {
        case EMovementState::None:
        case EMovementState::Grounded:
        case EMovementState::Mantling:
        case EMovementState::Ragdoll:
            if (MantleHeight > 125.0f)
            {
                MantleType = EMantleType::HighMantle;
            }
            else
            {
                MantleType = EMantleType::LowMantle;
            }
            break;
        case EMovementState::InAir:
            MantleType = EMantleType::FallingCatch;
            break;;
        default: ;
    }
    FGSComponentAndTransform ComponentAndTransform;
    ComponentAndTransform.Transform = TargetTransform;
    ComponentAndTransform.Component = HitComponent;
    MantleStart(MantleHeight, ComponentAndTransform, MantleType);
    return true;
}

void ASGCharacter::MantleStart(float MantleHeight, FGSComponentAndTransform& MantleLedgeWorldSpace, EMantleType MantleType)
{
    const UDAssetMantle* MantleAsset = GetMantleAsset(MantleType);
    if(nullptr == MantleAsset)
    {
        return;
    }
    MantleParams.AnimMontage = MantleAsset->AnimMontage;
    MantleParams.PositionCorrectionCurve = MantleAsset->PositionCorrectionCurve;
    MantleParams.StartingOffset = MantleAsset->StartingOffset;
    MantleParams.StartingPosition = UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset->LowHeight, MantleAsset->HighHeight, MantleAsset->LowStartPosition, MantleAsset->HighStartPosition);
    MantleParams.PlayRate = UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset->LowHeight, MantleAsset->HighHeight, MantleAsset->LowPlayRate, MantleAsset->HighPlayRate);

    {
        UGSBlueprintLibrary::ConvertWorldToLocal(MantleLedgeWorldSpace, MantleLedgeLocalSpace);
        MantleTarget = MantleLedgeWorldSpace.Transform;
        MantleActualStartOffset = UGSBlueprintLibrary::TransformSubtraction(GetActorTransform(), MantleTarget);
    }

    {
        FTransform A;
        FVector SubVector = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
        SubVector.Z = MantleParams.StartingOffset.Z;
        A.SetLocation(MantleTarget.GetLocation() - SubVector);
        A.SetRotation(MantleTarget.GetRotation());
        A.SetScale3D(FVector::OneVector);
        MantleAnimatedStartOffset = UGSBlueprintLibrary::TransformSubtraction(A, MantleTarget);
    }
    {
        if (auto LocalCharacterMovement = GetCharacterMovement())
        {
            LocalCharacterMovement->SetMovementMode(EMovementMode::MOVE_None, 0);
        }
        SetMovementState(EMovementState::Mantling);
    }

    {
        if (MantleTimeline && MantleParams.PositionCorrectionCurve)
        {
            float MinTime;
            float MaxTime;
            MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
            MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
            MantleTimeline->SetPlayRate(MantleParams.PlayRate);
            MantleTimeline->PlayFromStart();
        }
    }
    {
        if (IsValid(MantleParams.AnimMontage) && MainAnimInstance)
        {
            MainAnimInstance->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition, false);
        }
    }
}

void ASGCharacter::MantleEnd()
{
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        LocalCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking, 0);
    }
}

void ASGCharacter::MantleUpdate(float BlendIn)
{
    //Step 1: Continually update the mantle target from the stored local transform to follow along with moving objects.
    FGSComponentAndTransform MantleLedgeWorldSpace;
    UGSBlueprintLibrary::ConvertLocalToWorld(MantleLedgeLocalSpace, MantleLedgeWorldSpace);
    MantleTarget = MantleLedgeWorldSpace.Transform;

    float PositionAlpha = 0, XYCorrectionAlpha = 0, ZCorrectionAlpha = 0;
    //Step 2: Update the Position and Correction Alphas using the Position/Correction curve set for each Mantle.
    if (MantleTimeline && MantleParams.PositionCorrectionCurve)
    {
        FVector Result = MantleParams.PositionCorrectionCurve->GetVectorValue(MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
        PositionAlpha = Result.X;
        XYCorrectionAlpha = Result.Y;
        ZCorrectionAlpha = Result.Z;
    }

    //Step 3: Lerp multiple transforms together for independent control over the horizontal and vertical blend to the animated start position, as well as the target position.
    FVector AnimateStartLocation = MantleAnimatedStartOffset.GetLocation();
    FVector ActualStartLocation = MantleActualStartOffset.GetLocation();
    float Z = AnimateStartLocation.Z;
    AnimateStartLocation.Z = ActualStartLocation.Z;
    ActualStartLocation.Z = Z;
    FTransform AnimateTransform(MantleAnimatedStartOffset.GetRotation(), AnimateStartLocation, FVector::OneVector);
    FTransform AnimateLerp = UKismetMathLibrary::TLerp(MantleActualStartOffset, AnimateTransform, XYCorrectionAlpha);

    FTransform ActuaTransform(MantleActualStartOffset.GetRotation(), ActualStartLocation, FVector::OneVector);
    FTransform ActualLerp = UKismetMathLibrary::TLerp(MantleActualStartOffset, ActuaTransform, ZCorrectionAlpha);
    FVector Location = AnimateLerp.GetLocation();
    Location.Z = ActualLerp.GetLocation().Z;
    FTransform Result(AnimateLerp.GetRotation(), Location, FVector::OneVector);

    FTransform Temp = UKismetMathLibrary::TLerp(UGSBlueprintLibrary::TransformAddition(MantleTarget, Result), MantleTarget, PositionAlpha);
    FTransform LerpedTarget = UKismetMathLibrary::TLerp(UGSBlueprintLibrary::TransformAddition(MantleTarget, MantleActualStartOffset), Temp, BlendIn);

    //Step 4: Set the actors location and rotation to the Lerped Target.
    FHitResult SweepHitResult;
    SetActorLocationAndRotation(LerpedTarget.GetLocation(), LerpedTarget.Rotator(), false, false, SweepHitResult);
}

bool ASGCharacter::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType)
{
    if (Capsule)
    {
        float HalfHeight = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere();
        float Z = HalfHeight - RadiusOffset + HeightOffset;
        float Radius = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

        FVector Start = TargetLocation + FVector(0, 0, Z);
        FVector End = TargetLocation - FVector(0, 0, Z);
        TArray<AActor*> ActorIgnore;
        FHitResult HitResult;
        UKismetSystemLibrary::SphereTraceSingleByProfile(this, Start, End, Radius, "GS_Character", false, ActorIgnore, GetTraceDebugType(DebugType), HitResult, true, FColor::Green, FColor::Purple, 1.0f);

        return UKismetMathLibrary::BooleanNOR(HitResult.bBlockingHit, HitResult.bStartPenetrating);
    }
    return true;
}

const UDAssetMantle* ASGCharacter::GetMantleAsset(EMantleType MantleType) const
{
    if (auto FindItem = MantleAssetMap.FindRef(MantleType))
    {
        return FindItem;
    }
    return nullptr;
}

void ASGCharacter::RagdollStart()
{
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        LocalCharacterMovement->SetMovementMode(EMovementMode::MOVE_None, 0);
        SetMovementState(EMovementState::Ragdoll);

        if (auto LocalCapsuleComponent = GetCapsuleComponent())
        {
            LocalCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        if (auto LocalMesh = GetMesh())
        {
            LocalMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
            LocalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            LocalMesh->SetAllBodiesBelowSimulatePhysics(UKismetSystemLibrary::MakeLiteralName(Name_BonePelvis), true, true);
        }
        if (MainAnimInstance)
        {
            MainAnimInstance->Montage_Stop(0.2f);
        }
    }
}

void ASGCharacter::RagdollEnd()
{
    if (MainAnimInstance)
    {
        MainAnimInstance->SavePoseSnapshot(Name_RagdollPose);
    }
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        if (RagdollOnGround)
        {
            LocalCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking, 0);
            if (MainAnimInstance)
            {
                MainAnimInstance->Montage_Play(GetUpAnimation(RagdollFaceUp), 1.0f, EMontagePlayReturnType::MontageLength, 0, true);
            }
        }
        else
        {
            LocalCharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling, 0);
            LocalCharacterMovement->Velocity = LastRagdollVelocity;
        }
    }
    if (auto LocalCapsuleComponent = GetCapsuleComponent())
    {
        LocalCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        if (auto LocalMesh = GetMesh())
        {
            LocalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
            LocalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            LocalMesh->SetAllBodiesSimulatePhysics(false);
        }
    }
}

void ASGCharacter::RagdollUpdate()
{
    if (auto LocalMesh = GetMesh())
    {
        LastRagdollVelocity = LocalMesh->GetPhysicsLinearVelocity(Name_BoneRoot);
        float InSpring = UKismetMathLibrary::MapRangeClamped(LastRagdollVelocity.Size(), 0.f, 1000.f, 0.f, 25000.f);
        LocalMesh->SetAllMotorsAngularDriveParams(InSpring, 0, 0, false);
        LocalMesh->SetEnableGravity(LastRagdollVelocity.Z > -4000.f);
    }
    SetActorLocationDuringRagdoll();
}

void ASGCharacter::SetActorLocationDuringRagdoll()
{
    if (auto LocalMesh = GetMesh())
    {
        FVector TargetRagdollLocation = LocalMesh->GetSocketLocation(Name_BonePelvis);
        FRotator TargetRagdollRotation = LocalMesh->GetSocketRotation(Name_BonePelvis);
        RagdollFaceUp = TargetRagdollRotation.Roll < 0;
        if (RagdollFaceUp)
        {
            TargetRagdollRotation = FRotator(0, TargetRagdollRotation.Yaw - 180, 0);
        }
        else
        {
            TargetRagdollRotation = FRotator(0, TargetRagdollRotation.Yaw, 0);
        }

        if (auto LocalCapsuleComponent = GetCapsuleComponent())
        {
            float HalfHeight = LocalCapsuleComponent->GetScaledCapsuleHalfHeight();
            FVector End = TargetRagdollLocation;
            End.Z -= HalfHeight;
            TArray<AActor*> IgnoreActors;
            FHitResult HitResult;
            UKismetSystemLibrary::LineTraceSingle(this, TargetRagdollLocation, End, TraceTypeRagdoll, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true, FColor::Yellow, FColor::Yellow, 0);
            RagdollOnGround = HitResult.bBlockingHit;
            if (RagdollOnGround)
            {
                FVector Location = TargetRagdollLocation;
                Location.Z += HalfHeight - FMath::Abs(HitResult.ImpactPoint.Z - HitResult.TraceStart.Z);
                Location.Z += 2.0f;
                FHitResult SweepHitResult;
                SetActorLocationAndRotation(Location, TargetRagdollRotation, false, false, SweepHitResult);
            }
            else
            {
                FHitResult SweepHitResult;
                SetActorLocationAndRotation(TargetRagdollLocation, TargetRagdollRotation, false, false, SweepHitResult);
            }
        }
    }
}

UAnimMontage* ASGCharacter::GetUpAnimation(bool bRagdollFaceUp)
{
    if (DTRowOverlayState)
    {
        if (UConfigWorldSubsystem* Subsystem = UConfigWorldSubsystem::Get(this))
        {
            if (bRagdollFaceUp)
            {
                return Subsystem->GetCacheable(DTRowOverlayState->AnimMontageBackUp);
            }
            else
            {
                return Subsystem->GetCacheable(DTRowOverlayState->AnimMontageFrontUp);
            }
        }
    }
    return nullptr;
}

void ASGCharacter::DrawDebugShapes()
{
    if (auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
    }
}

EDrawDebugTrace::Type ASGCharacter::GetTraceDebugType(EDrawDebugTrace::Type DebugType)
{
    if (IsLocallyControlled())
    {
        return DebugType;
    }
    return EDrawDebugTrace::None;
}

void ASGCharacter::SetMovementState(EMovementState NewMovementState)
{
    if (MovementState != NewMovementState)
    {
        OnMovementStateChanged(NewMovementState);
    }
}

void ASGCharacter::SetMovementAction(EMovementAction NewMovementAction)
{
    if (MovementAction != NewMovementAction)
    {
        OnMovementActionChanged(NewMovementAction);
    }
}

void ASGCharacter::SetRotationMode(ERotationMode NewRotationMode)
{
    if (RotationMode != NewRotationMode)
    {
        OnRotationModeChanged(NewRotationMode);
    }
}

void ASGCharacter::SetGait(EGait NewGait)
{
    if (Gait != NewGait)
    {
        OnGaitChanged(NewGait);
    }
}

void ASGCharacter::SetViewMode(EViewMode NewViewMode)
{
    if (ViewMode != NewViewMode)
    {
        OnViewModeChanged(NewViewMode);
    }
}

void ASGCharacter::SetOverlayState(EOverlayState NewOverlayState)
{
    if (OverlayState != NewOverlayState)
    {
        OnOverlayStateChanged(NewOverlayState);
    }
}

void ASGCharacter::OnResetBrakingFrctionFactor()
{
    GetWorldTimerManager().ClearTimer(TimerResetBrakingFactor);
    if (auto LocalCharacterMovement = GetCharacterMovement())
    {
        LocalCharacterMovement->BrakingFrictionFactor = 0.0f;
    }
}

void ASGCharacter::OnMoveForward(float Val)
{
    PlayerMovementInput(true);
}

void ASGCharacter::OnMoveRight(float Val)
{
    PlayerMovementInput(false);
}

void ASGCharacter::OnLookUp(float Val)
{
    AddControllerPitchInput(LookUpDownRate * Val);
}

void ASGCharacter::OnLookRight(float Val)
{
    AddControllerYawInput(LookLeftRightRate * Val);
}

void ASGCharacter::OnJumpPressed()
{
    if (EMovementAction::None == MovementAction)
    {
        switch (MovementState)
        {
            case EMovementState::None:
                break;
            case EMovementState::Grounded:
                if (HasMovementInput)
                {
                   if(MantleCheck(FallingTraceSettings, EDrawDebugTrace::ForDuration))
                   {
                       return;
                   }
                }
                switch (Stance)
                {
                    case EStance::Standing:
                        Jump();
                        break;
                    case EStance::Crouching:
                        UnCrouch();
                        break;
                    default: ;
                }
                break;
            case EMovementState::InAir:
                MantleCheck(FallingTraceSettings, EDrawDebugTrace::ForDuration);
                break;
            case EMovementState::Mantling:
                break;
            case EMovementState::Ragdoll:
                RagdollEnd();
                break;
            default: ;
        }
    }
}

void ASGCharacter::OnJumpReleased()
{
    StopJumping();
}

void ASGCharacter::OnWalkPressed()
{
    switch (DesiredGait)
    {
        case EGait::Walking:
            DesiredGait = EGait::Running;
            break;
        case EGait::Running:
            DesiredGait = EGait::Walking;
            break;
        case EGait::Sprinting:
            break;
        default: ;
    }
}

void ASGCharacter::OnSelectRotationMode1Pressed()
{
    DesiredRotationMode = ERotationMode::VelocityDirection;
    SetRotationMode(DesiredRotationMode);
}

void ASGCharacter::OnSelectRotationMode2Pressed()
{
    DesiredRotationMode = ERotationMode::LookingDirection;
    SetRotationMode(DesiredRotationMode);
}

void ASGCharacter::OnAimPressed()
{
    SetRotationMode(ERotationMode::Aiming);
}

void ASGCharacter::OnAimReleased()
{
    switch (ViewMode)
    {
        case EViewMode::ThirdPerson:
            SetRotationMode(DesiredRotationMode);
            break;
        case EViewMode::FirstPerson:
            SetRotationMode(ERotationMode::LookingDirection);
            break;
        default: ;
    }
}

void ASGCharacter::OnStancePressed()
{

}

void ASGCharacter::OnCameraPressed()
{

}

void ASGCharacter::OnCameraReleased()
{
}

void ASGCharacter::OnSprintPressed()
{
    DesiredGait = EGait::Sprinting;
}

void ASGCharacter::OnSprintReleased()
{
    DesiredGait = EGait::Running;
}

void ASGCharacter::OnRagdollPressed()
{
    if(EMovementState::Ragdoll == MovementState)
    {
        RagdollEnd();
    }else
    {
        RagdollStart();
    }
}

// Called every frame
void ASGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetEssentialValues();
    switch (MovementState)
    {
        case EMovementState::Grounded:
            UpdateCharacterMovement();
            UpdateGroundedRotation();
            break;
        case EMovementState::InAir:
            UpdateInAirRotation();
            if (HasMovementInput)
            {
                MantleCheck(FallingTraceSettings, EDrawDebugTrace::ForOneFrame);
            }
            break;
        case EMovementState::Ragdoll:
            RagdollUpdate();
            break;
        default: ;
    }

    CacheValues();
    DrawDebugShapes();
}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis(GSInputName::MoveForwardBackward, this, &ASGCharacter::OnMoveForward);
        PlayerInputComponent->BindAxis(GSInputName::MoveRightLeft, this, &ASGCharacter::OnMoveRight);
        PlayerInputComponent->BindAxis(GSInputName::LookUpDown, this, &ASGCharacter::OnLookUp);
        PlayerInputComponent->BindAxis(GSInputName::LookLeftRight, this, &ASGCharacter::OnLookRight);

        PlayerInputComponent->BindAction(GSInputName::JumpAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnJumpPressed);
        PlayerInputComponent->BindAction(GSInputName::JumpAction, EInputEvent::IE_Released, this, &ASGCharacter::OnJumpReleased);
        PlayerInputComponent->BindAction(GSInputName::WalkAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnWalkPressed);
        PlayerInputComponent->BindAction(GSInputName::SelectRotationMode_1, EInputEvent::IE_Pressed, this, &ASGCharacter::OnSelectRotationMode1Pressed);
        PlayerInputComponent->BindAction(GSInputName::SelectRotationMode_2, EInputEvent::IE_Pressed, this, &ASGCharacter::OnSelectRotationMode2Pressed);

        PlayerInputComponent->BindAction(GSInputName::AimAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnAimPressed);
        PlayerInputComponent->BindAction(GSInputName::AimAction, EInputEvent::IE_Released, this, &ASGCharacter::OnAimReleased);

        PlayerInputComponent->BindAction(GSInputName::StanceAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnStancePressed);

        PlayerInputComponent->BindAction(GSInputName::CameraAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnCameraPressed);
        PlayerInputComponent->BindAction(GSInputName::CameraAction, EInputEvent::IE_Released, this, &ASGCharacter::OnCameraReleased);

        PlayerInputComponent->BindAction(GSInputName::SprintAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnSprintPressed);
        PlayerInputComponent->BindAction(GSInputName::SprintAction, EInputEvent::IE_Released, this, &ASGCharacter::OnSprintReleased);

        PlayerInputComponent->BindAction(GSInputName::RagdollAction, EInputEvent::IE_Pressed, this, &ASGCharacter::OnRagdollPressed);
    }
}

void ASGCharacter::GetControlForwardAndRightVector(FVector& Forward, FVector& Right)
{
    FRotator Rotator = GetControlRotation();
    Forward = UKismetMathLibrary::GetForwardVector(FRotator(0, 0, Rotator.Yaw));
    Right = UKismetMathLibrary::GetRightVector(FRotator(0, 0, Rotator.Yaw));
}

FVector ASGCharacter::GetCapsuleBaseLocation(float ZOffset)
{
    auto CapsuleComponentTemp = GetCapsuleComponent();
    if (CapsuleComponentTemp)
    {
        FVector UpVector = CapsuleComponentTemp->GetUpVector();
        float HalfHeight = CapsuleComponentTemp->GetScaledCapsuleHalfHeight();
        FVector WorldLocation = CapsuleComponentTemp->GetComponentLocation();
        return WorldLocation - UpVector * (ZOffset + HalfHeight);
    }
    return FVector::ZeroVector;
}

FVector ASGCharacter::GetCapsuleLocationFromBase(const FVector& BaseLocation, float ZOffset)
{
    auto CapsuleComponentTemp = GetCapsuleComponent();
    if (CapsuleComponentTemp)
    {
        float HalfHeight = CapsuleComponentTemp->GetScaledCapsuleHalfHeight();
        return BaseLocation + FVector(0, 0, ZOffset + HalfHeight);
    }
    return FVector::ZeroVector;
}

float ASGCharacter::GetAnimCurveValue(const FName& CurveName)
{
    if (MainAnimInstance)
    {
        return MainAnimInstance->GetCurveValue(CurveName);
    }
    return 0;
}


void ASGCharacter::PlayerMovementInput(bool IsForwardAxis)
{
    switch (MovementState)
    {
        case EMovementState::None:
            break;
        case EMovementState::Grounded:
        case EMovementState::InAir:
        {
            FVector ForwardVector;
            FVector RightVector;
            GetControlForwardAndRightVector(ForwardVector, RightVector);
            if (IsForwardAxis)
            {
            }
            else
            {
            }
        }
            break;
        case EMovementState::Mantling:
            break;
        case EMovementState::Ragdoll:
            break;
        default: ;
    }
}

FVector ASGCharacter::GetPlayerMovementInput()
{
    float MoveForward = GetInputAxisValue(GSInputName::MoveForwardBackward);
    float MoveRight = GetInputAxisValue(GSInputName::LookLeftRight);
    FVector ForwardVector;
    FVector RightVector;
    GetControlForwardAndRightVector(ForwardVector, RightVector);
    FVector Result = ForwardVector * MoveForward + RightVector * MoveRight;
    return Result.GetSafeNormal(0.0001f);
}

void ASGCharacter::FixDiagonalGamepadValues(float InX, float InY, float& OutX, float& OutY)
{
    float AbsInX = FMath::Abs(InX);
    float AbsInY = FMath::Abs(InY);
    float ClampedInX = FMath::GetMappedRangeValueClamped(TRange<float>(0, 0.6f), TRange<float>(1.0f, 1.2f), AbsInX);
    float ClampedInY = FMath::GetMappedRangeValueClamped(TRange<float>(0, 0.6f), TRange<float>(1.0f, 1.2f), AbsInY);
    OutY = FMath::Clamp(InY * ClampedInX, -1.0f, 1.0f);
    OutX = FMath::Clamp(InX * ClampedInY, -1.0f, 1.0f);
}

void ASGCharacter::SetEssentialValues()
{
    Acceleration = CalculateAcceleration();

    FVector Velocity = GetVelocity();
    Velocity.Z = 0;
    Speed = Velocity.Size();
    IsMoving = Speed > 1.0f;
    if (IsMoving)
    {
        LastVelocityRotation = GetVelocity().ToOrientationRotator();
    }

    if (auto CharacterMovementTemp = GetCharacterMovement())
    {
        MovementInputAmount = CharacterMovementTemp->GetCurrentAcceleration().Size() / CharacterMovementTemp->GetMaxAcceleration();
        HasMovementInput = MovementInputAmount > 0.0f;
        if (HasMovementInput)
        {
            LastMovementInputRotation = CharacterMovementTemp->GetCurrentAcceleration().ToOrientationRotator();
        }
    }

    AimYawRate = FMath::Abs((GetControlRotation().Yaw - PreviousAimYaw) / GetWorld()->GetDeltaSeconds());
}

void ASGCharacter::CacheValues()
{
    PreviousVelocity = GetVelocity();
    PreviousAimYaw = GetControlRotation().Yaw;
}

FVector ASGCharacter::CalculateAcceleration()
{
    return (GetVelocity() - PreviousVelocity) / (GetWorld()->GetDeltaSeconds());
}
