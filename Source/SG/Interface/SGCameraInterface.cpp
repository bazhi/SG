


#include "SGCameraInterface.h"

// Add default functionality here for any ISGCameraInterface functions that are not pure virtual.
void ISGCameraInterface::GetCameraParameters(float& TpFov, float& FpFov, bool& bRightShoulder)
{

}

FVector ISGCameraInterface::GetFpCameraTarget()
{
	return FVector::ZeroVector;
}

FTransform ISGCameraInterface::Get3PPivotTarget()
{
	return FTransform();
}

void ISGCameraInterface::Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, ETraceTypeQuery& TraceChannel)
{

}

