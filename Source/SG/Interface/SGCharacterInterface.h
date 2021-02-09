

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SG/Character/ECharacter.h"
#include "SGCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SG_API ISGCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetCurrentStates(EMovementMode& OutPawnMovementMode, EMovementState& OutMovementState, 
								  EMovementState& OutPrevMovementState, EMovementAction& OutMovementAction,
								  ERotationMode& OutRotationMode, EGait& OutActualGait, EStance& OutActualStance, 
								  EViewMode& OutViewMode, EOverlayState& OutOverlayState);

	virtual void GetEssentialValues(FVector& OutVelocity, FVector& OutAcceleration, FVector& OutMovementInput, bool& OutIsMoving, bool
									& OutHasMovementInput, float& OutSpeed, float& OutMovementInputAmount, FRotator& OutAimingRotation, float& OutAimYawRate);

	virtual void SetMovementState(EMovementState NewMovementState);
	virtual void SetMovementAction(EMovementAction NewMovementAction);
	virtual void SetRotationMode(ERotationMode NewRotationMode);
	virtual void SetGait(EGait NewGait);
	virtual void SetViewMode(EViewMode NewViewMode);
	virtual void SetOverlayState(EOverlayState NewOverlayState);
};
