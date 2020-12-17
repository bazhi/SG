#pragma once

#include "CoreMinimal.h"
#include "TableRowDefine.h"

#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"

#include "ConfigManager.generated.h"

struct FDataTableRow;
/**
 * 
 */
UCLASS()
class SG_API UConfigManager : public UDataAsset
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Configs")
    TSet<TSoftObjectPtr<UDataTable>> PreLoad;
#if WITH_EDITORONLY_DATA
    UPROPERTY(EditDefaultsOnly, Category = "Configs")
    TSet<TSoftObjectPtr<UDataTable>> DynamicLoad;
    UPROPERTY(EditDefaultsOnly, Category = "Configs")
    TSet<TSoftObjectPtr<UDataTable>> WorldLoad;
    UPROPERTY(EditDefaultsOnly, Category = "Configs")
    TSet<TSoftObjectPtr<UDataTable>> EditorLoad;
    UPROPERTY(VisibleDefaultsOnly, Category = "Configs")
    TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> EditorLoadMap;
#endif

    UPROPERTY(VisibleDefaultsOnly, Category = "Configs")
    TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> DynamicLoadMap;
    UPROPERTY(VisibleDefaultsOnly, Category = "Configs")
    TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> WorldLoadMap;
public:
    UDataTable* GetDataTable(UScriptStruct* RowStruct);
    UDataTable* GetWorldDataTable(UScriptStruct* RowStruct);
    TSet<TSoftObjectPtr<UDataTable>>& GetPreLoadTables();
protected:
#if WITH_EDITORONLY_DATA
    virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif

    void PreSaveTableMap(TSet<TSoftObjectPtr<UDataTable>>& LoadSet, TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>>& OutMap);
};
