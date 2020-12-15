#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSBlueprintLibrary.generated.h"

class UDataTable;
struct FGSComponentAndTransform;
/**
 * 
 */
UCLASS()
class SG_API UGSBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static void ConvertWorldToLocal(const FGSComponentAndTransform& WorldSpaceComponent, FGSComponentAndTransform& LocalSpaceComponent);
    static void ConvertLocalToWorld(const FGSComponentAndTransform& LocalSpaceComponent, FGSComponentAndTransform& WorldSpaceComponent);

    static FTransform TransformSubtraction(const FTransform& A, const FTransform B);
    static FTransform TransformAddition(const FTransform& A, const FTransform B);
};
