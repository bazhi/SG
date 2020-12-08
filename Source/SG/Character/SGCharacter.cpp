#include "SGCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GSInputName.h"
#include "Components/TimelineComponent.h"

#include "Curves/CurveVector.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"

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

void ASGCharacter::OnCharacteMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode, uint8 PrevCustomMode, uint8 NewCustomMode)
{
    switch (NewMovementMode)
    {
        case MOVE_Walking:
        case MOVE_NavWalking:
            MovementState = EMovementState::Grounded;
            break;
        case MOVE_Falling:
            MovementState = EMovementState::InAir;
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
                    ViewMode = EViewMode::ThirdPerson;
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
                    RotationMode = DesiredRotationMode;
                    break;
                default: ;
            }
            break;
        case EViewMode::FirstPerson:
            switch (RotationMode)
            {
                case ERotationMode::VelocityDirection:
                    RotationMode = ERotationMode::LookingDirection;
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
        Gait = ActualGait;
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
    if(SettingsStance)
    {
        switch (Stance)
        {
            case EStance::Standing:
                return SettingsStance->Standing;
            case EStance::Crouching:
                return SettingsStance->Crouching;
            default:;
        }
    }
    return FSGMovementSettings();
}

float ASGCharacter::GetMappedSpeed()
{
    float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
    float LocRunSpeed = CurrentMovementSettings.RunSpeed;
    float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;
    if(Speed <= LocWalkSpeed)
    {
        return UKismetMathLibrary::MapRangeClamped(Speed, 0.f, LocWalkSpeed, 0.f, 1.f);
    }else if(Speed <= LocRunSpeed)
    {
        return UKismetMathLibrary::MapRangeClamped(Speed, LocWalkSpeed, LocRunSpeed, 1.f, 2.f);
    }else
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

void ASGCharacter::RagdollStart()
{
}

// Called every frame
void ASGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
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
