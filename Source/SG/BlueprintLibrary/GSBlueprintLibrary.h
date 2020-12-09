#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSBlueprintLibrary.generated.h"

struct FGSComponentAndTransform;
/**
 * 
 */
UCLASS()
class SG_API UGSBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    static void ConvertWorldToLocal(const FGSComponentAndTransform& WorldSpaceComponent, FGSComponentAndTransform& LocalSpaceComponent);
    static void ConvertLocalToWorld(const FGSComponentAndTransform& LocalSpaceComponent, FGSComponentAndTransform& WorldSpaceComponent);
};
