

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGAnimationInterface.generated.h"

enum class EOverlayOverrideState : unsigned char;
enum class EOverlayState : unsigned char;
enum class EGroundedEntryState : unsigned char;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SG_API ISGAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnJumped();
	virtual void SetGroundedEntryState(EGroundedEntryState InGroundedEntryState);
	virtual void SetOverlayOverrideState(EOverlayOverrideState InOverlayOverrideState);
};
