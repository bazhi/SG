#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacter.h"

#include "Engine/DataTable.h"

#include "SGCharacter.generated.h"

UCLASS()
class SG_API ASGCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASGCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:


    //Utility
    void GetControlForwardAndRightVector(FVector& Forward, FVector& Right);
    FVector GetCapsuleBaseLocation(float ZOffset);
    FVector GetCapsuleLocationFromBase(const FVector& BaseLocation, float ZOffset);
    float GetAnimCurveValue(const FName& CurveName);

    //Input
    void PlayerMovementInput(bool IsForwardAxis);
    FVector GetPlayerMovementInput();
    void FixDiagonalGamepadValues(float InX, float InY, float& OutX, float& OutY);

    //Essential Information
    void SetEssentialValues();
    void CacheValues();
    FVector CalculateAcceleration();

    //State Changes
    void OnBeginPlay();
    void OnCharacteMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode, uint8 PrevCustomMode, uint8 NewCustomMode);
    void OnMovementStateChanged(EMovementState NewMovementState);
    void OnMovementActionChanged(EMovementAction NewMovementAction);
    void OnStanceChanged(EStance NewStance);
    void OnRotationModeChanged(ERotationMode NewRotationMode);
    void OnGaitChanged(EGait NewActualGait);
    void OnViewModeChanged(EViewMode NewViewMode);
    void OnOverlayStateChanged(EOverlayState NewOverlayState);

    //Movement System
    void SetMovementModel();
    void UpdateCharacterMovement();
    void UpdateDynamicMovementSettings(EGait AllowedGait);
    FSGMovementSettings GetTargetMovementSettings();
    float GetMappedSpeed();
    EGait GetAllowedGait();
    EGait GetActualGait(EGait AllowedGait);
    bool CanSprint();

    //RagdollSystem
    void RagdollStart();
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UTimelineComponent* MantleTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "References")
    class UAnimInstance* MainAnimInstance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    ERotationMode DesiredRotationMode = ERotationMode::LookingDirection;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    EGait DesiredGait = EGait::Running;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    EStance DesiredStance = EStance::Standing;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    float LookUpDownRate = 1.25f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    float LookLeftRightRate = 1.25f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    int32 TimesPressedStance = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    bool BreakFall = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    bool SprintHeld = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    FVector Acceleration = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    bool IsMoving = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    bool HasMovementInput = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    FRotator LastVelocityRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    FRotator LastMovementInputRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    float Speed = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    float MovementInputAmount = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential Information")
    float AimYawRate = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    float ThirdPersonFOV = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    float FirstPersonFOV = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    bool RightShoulder = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EMovementState MovementState = EMovementState::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EMovementState PrevMovementState = EMovementState::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EMovementAction MovementAction = EMovementAction::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    ERotationMode RotationMode = ERotationMode::LookingDirection;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EGait Gait = EGait::Walking;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EStance Stance = EStance::Standing;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EViewMode ViewMode = EViewMode::ThirdPerson;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
    EOverlayState OverlayState = EOverlayState::Default;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement System")
    FDataTableRowHandle MovementModel;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement System")
    FSGMovementSettingsState MovementData;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement System")
    FSGMovementSettings CurrentMovementSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotation System")
    FRotator TargetRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotation System")
    FRotator InAirRotation = FRotator::ZeroRotator;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotation System")
    float YawOffset = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSMantleParams MantleParams;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSComponentAndTransform MantleLedgeLS;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleTarget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleActualStartOffset;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleAnimatedStartOffset;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSMantleTraceSettings GroundedTraceSettings;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSMantleTraceSettings AutomaticTraceSettings;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSMantleTraceSettings FallingTraceSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll System")
    bool RagdollOnGround = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll System")
    bool RagdollFaceUp = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll System")
    FVector LastRagdollVelocity = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cached Variables")
    FVector PreviousVelocity = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cached Variables")
    float PreviousAimYaw = 0;
};
