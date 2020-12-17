#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    class UConfigAsset* GetConfigManager()
    {
        return ConfigManager;
    }
protected:
    UPROPERTY(EditDefaultsOnly, Category = "DataAssets")
    class UConfigAsset* ConfigManager;
};
