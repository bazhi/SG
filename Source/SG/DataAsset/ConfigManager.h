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
    TSet<TSoftObjectPtr<UDataTable>> EditorLoad;
    UPROPERTY(VisibleDefaultsOnly, Category = "Configs")
    TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> EditorLoadMap;
#endif

    UPROPERTY(VisibleDefaultsOnly, Category = "Configs")
    TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> DynamicLoadMap;

private:
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> PreLoadTables;
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> DynamicLoadTables;
    UPROPERTY(Transient)
    TSet<UObject*> CachedObjects;
protected:
#if WITH_EDITORONLY_DATA
    virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
public:
    void OnStartupLoad();

    template <class T=FDataTableRow>
    UDataTable* GetDataTable();

    template <class T=FDataTableRow>
    T* GetDataTableRow(const T& Item);

    void EmptyDynamicTables()
    {
        DynamicLoadTables.Empty();
    };

    template <class T= TSoftObjectPtr<UObject>>
    T* GetCacheable(TSoftObjectPtr<T>& Item)
    {
        if (auto Object = Item.Get())
        {
            return Object;
        }

        if (auto Object = Item.LoadSynchronous())
        {
            CachedObjects.Add(Object);
            return Object;
        }
        return nullptr;
    }

private:
    FString ContextString = TEXT("UConfigManager");
};

template <class T>
 UDataTable* UConfigManager::GetDataTable()
{
    UScriptStruct* RowStruct = T::StaticStruct();
    if (RowStruct)
    {
        if (UDataTable* DataTable = PreLoadTables.FindRef(RowStruct))
        {
            return DataTable;
        }
        if (UDataTable* DataTable = DynamicLoadTables.FindRef(RowStruct))
        {
            return DataTable;
        }
        if (auto SoftObject = DynamicLoadMap.FindRef(RowStruct))
        {
            if (SoftObject.IsValid())
            {
                if (UDataTable* DataTable = SoftObject.LoadSynchronous())
                {
                    DynamicLoadTables.Emplace(RowStruct, DataTable);
                    return DataTable;
                }
            }
        }
#if WITH_EDITORONLY_DATA
        if (auto SoftObject = EditorLoadMap.FindRef(RowStruct))
        {
            if (SoftObject.IsValid())
            {
                if (UDataTable* DataTable = SoftObject.LoadSynchronous())
                {
                    DynamicLoadTables.Emplace(RowStruct, DataTable);
                    return DataTable;
                }
            }
        }
#endif
    }
    return nullptr;
}

 template <class T>
 T* UConfigManager::GetDataTableRow(const T& Item)
 {
     if (const UDataTable* DataTable = GetDataTable<T>())
     {
         return DataTable->FindRow<T>(Item.GetRowName(), ContextString);
     }
     return nullptr;
 }


