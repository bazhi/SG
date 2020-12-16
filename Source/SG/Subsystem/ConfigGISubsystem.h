#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"


#include "SG/DataAsset/ConfigManager.h"
#include "SG/DataAsset/TableRowDefine.h"


#include "Subsystems/GameInstanceSubsystem.h"
#include "ConfigGISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigGISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    static UConfigGISubsystem* Get(UGameInstance* GameInstance);

protected:
    void EmptyDynamicTables()
    {
        DynamicLoadTables.Empty();
    };

    void ClearCachedObjects();
public:
    template <class T = TSoftObjectPtr<UObject>>
    T* GetCacheable(TSoftObjectPtr<T>& Item);
    template <class T = FDataTableRow>
    UDataTable* GetDataTable();
    template <class T = FDataTableRow>
    T* GetDataTableRow(const T& Item);

private:
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> PreLoadTables;
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> DynamicLoadTables;
    UPROPERTY(Transient)
    TSet<UObject*> CachedObjects;

    FString ContextString = TEXT("UConfigManager");

    TWeakObjectPtr<UConfigManager> ConfigManager;
};

template <class T>
T* UConfigGISubsystem::GetCacheable(TSoftObjectPtr<T>& Item)
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

template <class T>
UDataTable* UConfigGISubsystem::GetDataTable()
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
        if(ConfigManager.IsValid())
        {
            if(UDataTable* DataTable = ConfigManager->GetDataTable(RowStruct))
            {
                DynamicLoadTables.Emplace(RowStruct, DataTable);
                return DataTable;
            }
        }
    }
    return nullptr;
}

template <class T>
T* UConfigGISubsystem::GetDataTableRow(const T& Item)
{
    if (const UDataTable* DataTable = GetDataTable<T>())
    {
        return DataTable->FindRow<T>(Item.GetRowName(), ContextString);
    }
    return nullptr;
}
