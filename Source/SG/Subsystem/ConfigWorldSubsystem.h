#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DataTable.h"

#include "ConfigWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

    friend class UConfigGameSubsystem;
protected:
    template <class T = TSoftObjectPtr<UObject>>
    T* GetCacheable(TSoftObjectPtr<T>& Item);

    UPROPERTY(Transient)
    TMap<const UScriptStruct*, UDataTable*> WorldLoadTables;
    UPROPERTY(Transient)
    TSet<UObject*> CachedObjects;
};

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
