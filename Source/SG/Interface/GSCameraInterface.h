

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GSCameraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGSCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SG_API IGSCameraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetCameraParameters(float& TpFov, float& FpFov, bool& bRightShoulder);
	virtual FVector GetFpCameraTarget();
	virtual FTransform Get3PPivotTarget();
	virtual void Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, ETraceTypeQuery& TraceChannel);
};
