#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SG/Character/ECharacter.h"
#include "SG/Interface/SGCameraInterface.h"
#include "SG/Interface/SGCharacterInterface.h"


#include "SGCameraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGCameraAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    void UpdateController(APlayerController* Controller, APawn* Pawn);
protected:
    void UpdateCharacterInfo();
protected:
    TWeakObjectPtr<APlayerController> PlayerController = nullptr;
    TWeakObjectPtr<APawn> ControlledPawn = nullptr;

    UPROPERTY(Transient)
    TScriptInterface<ISGCharacterInterface> ICharacter = nullptr;
    UPROPERTY(Transient)
    TScriptInterface<ISGCameraInterface> ICamera = nullptr;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    EMovementState MovementState = EMovementState::None;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    EMovementAction MovementAction = EMovementAction::None;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    ERotationMode RotationMode = ERotationMode::LookingDirection;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    EGait Gait = EGait::Walking;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    EStance Stance = EStance::Standing;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    EViewMode ViewMode = EViewMode::ThirdPerson;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bRightShoulder = true;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    bool bDebugView = true;
};
