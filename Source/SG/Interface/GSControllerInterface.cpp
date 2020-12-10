


#include "GSControllerInterface.h"

// Add default functionality here for any IGSControllerInterface functions that are not pure virtual.
ACharacter* IGSControllerInterface::GetDebugInfo(bool& bDebugView, bool& bShowHUD, bool& bShowTraces, bool& bShowDebugShapes, bool& bShowLayerColors, bool& bSlomo, bool& bShowCharacterInfo)
{
	bDebugView = false;
	bShowHUD = false;
	bShowTraces = false;
	bShowDebugShapes = false;
	bShowLayerColors = false;
	bSlomo = false;
	bShowCharacterInfo = false;
	return nullptr;
}

