#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DataTable.h"
#include "SG/DataAsset/ConfigManager.h"
#include "SG/DataAsset/TableRowDefine.h"
#include "ConfigWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    static UConfigWorldSubsystem* Get(const UObject* Context);

public:
    template <class T = FDataTableRow>
    UDataTable* GetDataTable();
    template <class T = FDataTableRow>
    T* GetDataTableRow(const T& Item);

protected:
    FORCEINLINE_DEBUGGABLE UDataTable* GetDataTable(UScriptStruct* RowStruct);

    class UConfigGameSubsystem* GetConfigGameSubsystem();
public:
    template <class T = TSoftObjectPtr<UObject>>
    T* GetCacheable(TSoftObjectPtr<T>& Item);
protected:
    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> WorldLoadTables;
    UPROPERTY(Transient)
    TSet<UObject*> CachedObjects;

    FString ContextString = TEXT("UConfigManager");

    TWeakObjectPtr<UConfigManager> ConfigManager;
    TWeakObjectPtr<class UConfigGameSubsystem> ConfigGameSubsystem;
};

template <class T>
UDataTable* UConfigWorldSubsystem::GetDataTable()
{
    UScriptStruct* RowStruct = T::StaticStruct();
    return GetDataTable(RowStruct);
}

template <class T>
T* UConfigWorldSubsystem::GetDataTableRow(const T& Item)
{
    UScriptStruct* RowStruct = T::StaticStruct();
    if (const UDataTable* DataTable = GetDataTable(RowStruct))
    {
        return DataTable->FindRow<T>(Item.GetRowName(), ContextString);
    }
    return nullptr;
}

template <class T>
T* UConfigWorldSubsystem::GetCacheable(TSoftObjectPtr<T>& Item)
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
