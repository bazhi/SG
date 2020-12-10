


#include "GSCameraInterface.h"

// Add default functionality here for any IGSCameraInterface functions that are not pure virtual.
void IGSCameraInterface::GetCameraParameters(float& TpFov, float& FpFov, bool& bRightShoulder)
{

}

FVector IGSCameraInterface::GetFpCameraTarget()
{
	return FVector::ZeroVector;
}

FTransform IGSCameraInterface::Get3PPivotTarget()
{
	return FTransform();
}

void IGSCameraInterface::Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, ETraceTypeQuery& TraceChannel)
{

}

