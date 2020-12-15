#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGEditorBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SGEDITOR_API USGEditorBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "DataTable")
    static bool RenameRowsName(UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "DataTable")
    static bool SortDataTable(UDataTable* DataTable);
};
