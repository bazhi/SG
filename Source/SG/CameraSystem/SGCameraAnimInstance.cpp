#include "SGCameraAnimInstance.h"

void USGCameraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    UpdateCharacterInfo();
}

void USGCameraAnimInstance::UpdateController(APlayerController* Controller, APawn* Pawn)
{
    ControlledPawn = Pawn;
    ICharacter = Pawn;
    ICamera = Pawn;

    PlayerController = Controller;
}

void USGCameraAnimInstance::UpdateCharacterInfo()
{
    if(ControlledPawn.IsValid())
    {
        if(ICharacter)
        {
            EMovementMode MovementMode;
            EMovementState PreMovementState;
            EOverlayState OverlayState;
            ICharacter->GetCurrentStates(MovementMode, MovementState, PreMovementState, MovementAction, RotationMode, Gait, Stance, ViewMode, OverlayState);
        }
        if(ICamera)
        {
            float TP_FOV;
            float FP_FOV;
            ICamera->GetCameraParameters(TP_FOV, FP_FOV, bRightShoulder);
        }
    }
}
