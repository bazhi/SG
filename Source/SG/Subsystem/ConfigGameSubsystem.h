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
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    static UConfigGameSubsystem* Get(UGameInstance* GameInstance);

    template <class T = TSoftObjectPtr<UObject>>
    T* GetCacheable(TSoftObjectPtr<T>& Item);

protected:
    void EmptyDynamicTables()
    {
        DynamicLoadTables.Empty();
    };
public:
    template <class T = FDataTableRow>
    UDataTable* GetDataTable();
    template <class T = FDataTableRow>
    T* GetDataTableRow(const T& Item);

private:
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> PreLoadTables;
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> DynamicLoadTables;
    FString ContextString = TEXT("UConfigManager");

    TWeakObjectPtr<UConfigManager> ConfigManager;
};

template <class T>
T* UConfigGameSubsystem::GetCacheable(TSoftObjectPtr<T>& Item)
{
    if (auto Object = Item.Get())
    {
        return Object;
    }

    return GetWorld()->GetSubsystem<UConfigWorldSubsystem>()->GetCacheable(Item);
}

template <class T>
UDataTable* UConfigGameSubsystem::GetDataTable()
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
        if (ConfigManager.IsValid())
        {
            if (UDataTable* DataTable = ConfigManager->GetDataTable(RowStruct))
            {
                DynamicLoadTables.Emplace(RowStruct, DataTable);
                return DataTable;
            }
        }

        UConfigWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UConfigWorldSubsystem>();
        if(WorldSubsystem)
        {
            if (UDataTable* DataTable = WorldSubsystem->WorldLoadTables.FindRef(RowStruct))
            {
                return DataTable;
            }
            if (ConfigManager.IsValid())
            {
                if (UDataTable* DataTable = ConfigManager->GetWorldDataTable(RowStruct))
                {
                    WorldSubsystem->WorldLoadTables.Emplace(RowStruct, DataTable);
                    return DataTable;
                }
            }
        }
    }
    return nullptr;
}

template <class T>
T* UConfigGameSubsystem::GetDataTableRow(const T& Item)
{
    if (const UDataTable* DataTable = GetDataTable<T>())
    {
        return DataTable->FindRow<T>(Item.GetRowName(), ContextString);
    }
    return nullptr;
}
