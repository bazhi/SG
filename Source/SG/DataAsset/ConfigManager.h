#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"

#include "ConfigManager.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigManager : public UDataAsset
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configs")
    TArray<TSoftObjectPtr<UDataTable>> ConfigArray;

public:
};
