


#include "SGCharacterInterface.h"

// Add default functionality here for any ISGCharacterInterface functions that are not pure virtual.
void ISGCharacterInterface::GetCurrentStates(EMovementMode& OutPawnMovementMode, EMovementState& OutMovementState, EMovementState& OutPrevMovementState, EMovementAction& OutMovementAction, ERotationMode& OutRotationMode, EGait& OutActualGait, EStance& OutActualStance, EViewMode& OutViewMode, EOverlayState& OutOverlayState)
{
}

void ISGCharacterInterface::GetEssentialValues(FVector& OutVelocity, FVector& OutAcceleration, FVector& OutMovementInput, bool& OutIsMoving, bool& OutHasMovementInput, float& OutSpeed, float& OutMovementInputAmount, FRotator& OutAimingRotation, float& OutAimYawRate)
{
}

void ISGCharacterInterface::SetMovementState(EMovementState NewMovementState)
{
}

void ISGCharacterInterface::SetMovementAction(EMovementAction NewMovementAction)
{
}

void ISGCharacterInterface::SetRotationMode(ERotationMode NewRotationMode)
{
}

void ISGCharacterInterface::SetGait(EGait NewGait)
{
}

void ISGCharacterInterface::SetViewMode(EViewMode NewViewMode)
{
}

void ISGCharacterInterface::SetOverlayState(EOverlayState NewOverlayState)
{
}
