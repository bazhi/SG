#include "ConfigAsset.h"
#include "Engine/StreamableManager.h"

UDataTable* UConfigAsset::GetDataTable(UScriptStruct* RowStruct)
{
    if (auto SoftObject = DynamicLoadMap.FindRef(RowStruct))
    {
        if (SoftObject.IsValid())
        {
            if (UDataTable* DataTable = SoftObject.LoadSynchronous())
            {
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
                return DataTable;
            }
        }
    }
#endif
    return nullptr;
}

UDataTable* UConfigAsset::GetWorldDataTable(UScriptStruct* RowStruct)
{
    if (auto SoftObject = WorldLoadMap.FindRef(RowStruct))
    {
        if (SoftObject.IsValid())
        {
            if (UDataTable* DataTable = SoftObject.LoadSynchronous())
            {
                return DataTable;
            }
        }
    }
    return nullptr;
}

TSet<TSoftObjectPtr<UDataTable>>& UConfigAsset::GetPreLoadTables()
{
    return PreLoad;
}

#if WITH_EDITORONLY_DATA
void UConfigAsset::PreSave(const ITargetPlatform* TargetPlatform)
{
    PreLoad.StableSort([](const TSoftObjectPtr<UDataTable>& A, const TSoftObjectPtr<UDataTable>& B)
    {
        return A.GetAssetName() > B.GetAssetName();
    });

    PreSaveTableMap(DynamicLoad, DynamicLoadMap);
    PreSaveTableMap(EditorLoad, EditorLoadMap);
    PreSaveTableMap(WorldLoad, WorldLoadMap);
    Super::PreSave(TargetPlatform);
}
#endif


void UConfigAsset::PreSaveTableMap(TSet<TSoftObjectPtr<UDataTable>>& LoadSet, TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>>& OutMap)
{
    LoadSet.StableSort([](const TSoftObjectPtr<UDataTable>& A, const TSoftObjectPtr<UDataTable>& B)
    {
        return A.GetAssetName() > B.GetAssetName();
    });
    OutMap.Empty();
    for (auto& SoftObject : LoadSet)
    {
        UDataTable* DataTable = SoftObject.LoadSynchronous();
        if (DataTable)
        {
            OutMap.Emplace(DataTable->GetRowStruct(), SoftObject);
        }
    }
}