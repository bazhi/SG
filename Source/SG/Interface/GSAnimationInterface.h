

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GSAnimationInterface.generated.h"

enum class EOverlayState : unsigned char;
enum class EGroundedEntryState : unsigned char;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGSAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SG_API IGSAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Jumped();
	virtual void SetGroundedEntryState(EGroundedEntryState GroundedEntryState);
	virtual void SetOverlayOverrideState(EOverlayState OverlayOverrideState);
};