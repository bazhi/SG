#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacter.h"

#include "Engine/DataTable.h"

#include "Kismet/KismetSystemLibrary.h"


#include "SG/DataAsset/DAssetMantle.h"
#include "SG/DataAsset/TableRowDefine.h"
#include "SG/GameInstance/SGGameInstance.h"
#include "SG/Interface/GSCameraInterface.h"
#include "SG/Interface/GSCharacterInterface.h"


#include "SGCharacter.generated.h"

UCLASS()
class SG_API ASGCharacter : public ACharacter, public IGSCharacterInterface, public IGSCameraInterface
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
    void UpdateMovementSetting();
    float GetMappedSpeed();
    EGait GetAllowedGait();
    EGait GetActualGait(EGait AllowedGait);
    bool CanSprint();
    UAnimMontage* GetRollAnimation() const;

    //Rotation System
    void UpdateGroundedRotation();
    void UpdateInAirRotation();
    void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed);
    void AddToCharacterRotation(FRotator DeltaRotation);
    void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed);
    bool SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult& SweepHitResult);
    float CalculateGroundedRotationRate();
    bool CanUpdateMovingRotation();

    //Mantle System
    bool MantleCheck(FSGMantleTraceSettings TraceSettings, EDrawDebugTrace::Type DebugType); //Can Climb/Vault
    void MantleStart(float MantleHeight, FGSComponentAndTransform& MantleLedgeWorldSpace, EMantleType MantleType);
    void MantleEnd();
    void MantleUpdate(float BlendIn);
    bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType);
    const UDAssetMantle* GetMantleAsset(EMantleType MantleType) const;

    //Ragdoll System
    void RagdollStart();
    void RagdollEnd();
    void RagdollUpdate();
    void SetActorLocationDuringRagdoll();
    UAnimMontage* GetUpAnimation(bool bRagdollFaceUp);
    //Debug
    void DrawDebugShapes();
    EDrawDebugTrace::Type GetTraceDebugType(EDrawDebugTrace::Type DebugType);

    //Events
    void OnBeginPlay();
    virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
    virtual void OnJumped_Implementation() override;
    virtual void Landed(const FHitResult& Hit) override;

    void BreakFallEvent();
    void RollEvent();

    virtual void SetMovementState(EMovementState NewMovementState) override;
    virtual void SetMovementAction(EMovementAction NewMovementAction) override;
    virtual void SetRotationMode(ERotationMode NewRotationMode) override;
    virtual void SetGait(EGait NewGait) override;
    virtual void SetViewMode(EViewMode NewViewMode) override;
    virtual void SetOverlayState(EOverlayState NewOverlayState) override;

protected:
    void OnResetBrakingFrctionFactor();

    //Axis Action
    void OnMoveForward(float Val);
    void OnMoveRight(float Val);
    void OnLookUp(float Val);
    void OnLookRight(float Val);

    //Button Action
    void OnJumpPressed();
    void OnJumpReleased();
    void OnWalkPressed();
    void OnSelectRotationMode1Pressed();
    void OnSelectRotationMode2Pressed();
    void OnAimPressed();
    void OnAimReleased();
    void OnStancePressed();
    void OnCameraPressed();
    void OnCameraReleased();
    void OnSprintPressed();
    void OnSprintReleased();
    void OnRagdollPressed();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UTimelineComponent* MantleTimeline;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "References")
    class UAnimInstance* MainAnimInstance;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Input")
    ERotationMode DesiredRotationMode = ERotationMode::LookingDirection;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Input")
    EGait DesiredGait = EGait::Running;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Input")
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

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    FVector Acceleration = FVector::ZeroVector;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    bool IsMoving = false;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    bool HasMovementInput = false;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    FRotator LastVelocityRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    FRotator LastMovementInputRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    float Speed = 0;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    float MovementInputAmount = 0;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Essential Information")
    float AimYawRate = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    float ThirdPersonFOV = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    float FirstPersonFOV = 90;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera System")
    bool RightShoulder = false;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EMovementState MovementState = EMovementState::None;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EMovementState PrevMovementState = EMovementState::None;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EMovementAction MovementAction = EMovementAction::None;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    ERotationMode RotationMode = ERotationMode::LookingDirection;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EGait Gait = EGait::Walking;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EStance Stance = EStance::Standing;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EViewMode ViewMode = EViewMode::ThirdPerson;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "State Values")
    EOverlayState OverlayState = EOverlayState::Default;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Rotation System")
    FRotator TargetRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Rotation System")
    FRotator InAirRotation = FRotator::ZeroRotator;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Rotation System")
    float YawOffset = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FSGMantleParams MantleParams;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FGSComponentAndTransform MantleLedgeLocalSpace;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleTarget;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleActualStartOffset;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Mantle System")
    FTransform MantleAnimatedStartOffset;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FSGMantleTraceSettings GroundedTraceSettings;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FSGMantleTraceSettings AutomaticTraceSettings;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    FSGMantleTraceSettings FallingTraceSettings;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mantle System")
    TMap<EMantleType, UDAssetMantle*> MantleAssetMap;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Ragdoll System")
    bool RagdollOnGround = false;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Ragdoll System")
    bool RagdollFaceUp = false;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Ragdoll System")
    FVector LastRagdollVelocity = FVector::ZeroVector;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Cached Variables")
    FVector PreviousVelocity = FVector::ZeroVector;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Cached Variables")
    float PreviousAimYaw = 0;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ETraceTypeQuery")
    TEnumAsByte<ETraceTypeQuery> TraceTypeClimbable;
    //Visibility
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ETraceTypeQuery")
    TEnumAsByte<ETraceTypeQuery> TraceTypeRagdoll;

protected:
    FDTRowOverlayState* DTRowOverlayState;

    FDTRowMovementSetting* DTRowMovementSetting;
private:
    FTimerHandle TimerResetBrakingFactor;
};
