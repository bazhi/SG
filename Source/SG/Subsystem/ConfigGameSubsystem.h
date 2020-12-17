#pragma once

#include "ConfigWorldSubsystem.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SG/DataAsset/ConfigManager.h"
#include "SG/DataAsset/TableRowDefine.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ConfigGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

    friend class UConfigWorldSubsystem;
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

protected:
    void EmptyDynamicTables()
    {
        DynamicLoadTables.Empty();
    };

    FORCEINLINE_DEBUGGABLE UDataTable* GetDataTable(UScriptStruct* RowStruct);

    TWeakObjectPtr<UConfigManager> ConfigManager;
private:
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> PreLoadTables;
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> DynamicLoadTables;
    FString ContextString = TEXT("UConfigManager");
};
