#include "ConfigWorldSubsystem.h"
#include "ConfigGameSubsystem.h"

void UConfigWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

UConfigWorldSubsystem* UConfigWorldSubsystem::Get(const UObject* Context)
{
    if (Context)
    {
        return Context->GetWorld()->GetSubsystem<UConfigWorldSubsystem>();
    }
    return nullptr;
}

UDataTable* UConfigWorldSubsystem::GetDataTable(UScriptStruct* RowStruct)
{
    if (RowStruct)
    {
        if (GetConfigGameSubsystem())
        {
            if (UDataTable* DataTable = ConfigGameSubsystem->GetDataTable(RowStruct))
            {
                return DataTable;
            }
        }
        if (UDataTable* DataTable = WorldLoadTables.FindRef(RowStruct))
        {
            return DataTable;
        }
        if (ConfigAsset.IsValid())
        {
            if (UDataTable* DataTable = ConfigAsset->GetDataTable(RowStruct))
            {
                WorldLoadTables.Emplace(RowStruct, DataTable);
                return DataTable;
            }
        }
    }
    return nullptr;
}

UConfigGameSubsystem* UConfigWorldSubsystem::GetConfigGameSubsystem()
{
    if(!ConfigGameSubsystem.IsValid())
    {
        UGameInstance* GameInstance = GetWorld()->GetGameInstance();
        if (GameInstance)
        {
            ConfigGameSubsystem = GameInstance->GetSubsystem<UConfigGameSubsystem>();
            if (ConfigGameSubsystem.IsValid())
            {
                ConfigAsset = ConfigGameSubsystem->ConfigAsset;
            }
        }
    }
    return ConfigGameSubsystem.Get();
}
