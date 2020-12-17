#include "ConfigGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SG/DataAsset/ConfigAsset.h"
#include "SG/GameInstance/SGGameInstance.h"

void UConfigGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if(USGGameInstance* GSGameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
		ConfigAsset = GSGameInstance->GetConfigManager();
	}
	if(ConfigAsset.IsValid())
	{
		for (auto SoftObject : ConfigAsset->GetPreLoadTables())
		{
			UDataTable* DataTable = SoftObject.LoadSynchronous();
			if (DataTable)
			{
				PreLoadTables.Emplace(DataTable->GetRowStruct(), DataTable);
			}
		}
	}
}

void UConfigGameSubsystem::Deinitialize()
{

}

UDataTable* UConfigGameSubsystem::GetDataTable(UScriptStruct* RowStruct)
{
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
        if (ConfigAsset.IsValid())
        {
            if (UDataTable* DataTable = ConfigAsset->GetDataTable(RowStruct))
            {
                DynamicLoadTables.Emplace(RowStruct, DataTable);
                return DataTable;
            }
        }
    }
    return nullptr;
}

