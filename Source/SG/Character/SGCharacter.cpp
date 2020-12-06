#include "SGCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GSInputName.h"
#include "Components/TimelineComponent.h"
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
                    if(MantleTimeline)
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
