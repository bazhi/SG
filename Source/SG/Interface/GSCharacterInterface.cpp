


#include "GSCharacterInterface.h"

// Add default functionality here for any IGSCharacterInterface functions that are not pure virtual.
void IGSCharacterInterface::GetCurrentStates(EMovementMode& OutPawnMovementMode, EMovementState& OutMovementState, EMovementState& OutPrevMovementState, EMovementAction& OutMovementAction, ERotationMode& OutRotationMode, EGait& OutActualGait, EStance& OutActualStance, EViewMode& OutViewMode, EOverlayState& OutOverlayState)
{
}

void IGSCharacterInterface::GetEssentialValues(FVector& OutVelocity, FVector& OutAcceleration, FVector& OutMovementInput, bool& OutIsMoving, bool& OutHasMovementInput, float& OutSpeed, float& OutMovementInputAmount, FRotator& OutAimingRotation, float& OutAimYawRate)
{
}

void IGSCharacterInterface::SetMovementState(EMovementState NewMovementState)
{
}

void IGSCharacterInterface::SetMovementAction(EMovementAction NewMovementAction)
{
}

void IGSCharacterInterface::SetRotationMode(ERotationMode NewRotationMode)
{
}

void IGSCharacterInterface::SetGait(EGait NewGait)
{
}

void IGSCharacterInterface::SetViewMode(EViewMode NewViewMode)
{
}

void IGSCharacterInterface::SetOverlayState(EOverlayState NewOverlayState)
{
}
