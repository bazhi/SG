#pragma once
#include <string>

#include "UObject/NameTypes.h"
#include "CoreMinimal.h"
#include "GSInputName.generated.h"

class GSInputName
{
public:
    static class FName MoveForwardBackward = "MoveForward/Backwards";
    static class FName MoveRightLeft = "MoveRight/Left";
    static class FName LookUpDown = "LookUp/Down";
    static class FName LookLeftRight = "LookLeft/Right";
};
