#include "ConfigManager.h"

#include "Engine/StreamableManager.h"

UDataTable* UConfigManager::GetDataTable(UScriptStruct* RowStruct)
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

TSet<TSoftObjectPtr<UDataTable>>& UConfigManager::GetPreLoadTables()
{
    return PreLoad;
}

#if WITH_EDITORONLY_DATA
void UConfigManager::PreSave(const ITargetPlatform* TargetPlatform)
{
    DynamicLoadMap.Empty();
    EditorLoadMap.Empty();
    PreLoad.StableSort([](const TSoftObjectPtr<UDataTable>& A, const TSoftObjectPtr<UDataTable>& B)
    {
        return A.GetAssetName() > B.GetAssetName();
    });
    DynamicLoad.StableSort([](const TSoftObjectPtr<UDataTable>& A, const TSoftObjectPtr<UDataTable>& B)
    {
        return A.GetAssetName() > B.GetAssetName();
    });
    EditorLoad.StableSort([](const TSoftObjectPtr<UDataTable>& A, const TSoftObjectPtr<UDataTable>& B)
    {
        return A.GetAssetName() > B.GetAssetName();
    });

    for (auto SoftObject : DynamicLoad)
    {
        UDataTable* DataTable = SoftObject.LoadSynchronous();
        if (DataTable)
        {
            DynamicLoadMap.Emplace(DataTable->GetRowStruct(), SoftObject);
        }
    }

    for (auto SoftObject : EditorLoad)
    {
        UDataTable* DataTable = SoftObject.LoadSynchronous();
        if (DataTable)
        {
            EditorLoadMap.Emplace(DataTable->GetRowStruct(), SoftObject);
        }
    }

    Super::PreSave(TargetPlatform);
}


#endif
