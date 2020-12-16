


#include "ConfigGISubsystem.h"


#include "SG/DataAsset/ConfigManager.h"
#include "SG/GameInstance/SGGameInstance.h"

void UConfigGISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if(USGGameInstance* GSGameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
		ConfigManager = GSGameInstance->GetConfigManager();
	}
	if(ConfigManager.IsValid())
	{
		for (auto SoftObject : ConfigManager->GetPreLoadTables())
		{
			UDataTable* DataTable = SoftObject.LoadSynchronous();
			if (DataTable)
			{
				PreLoadTables.Emplace(DataTable->GetRowStruct(), DataTable);
			}
		}
	}
}

void UConfigGISubsystem::Deinitialize()
{

}

UConfigGISubsystem* UConfigGISubsystem::Get(UGameInstance* GameInstance)
{
	if(GameInstance)
	{
		return GameInstance->GetSubsystem<UConfigGISubsystem>();
	}
	return nullptr;
}

void UConfigGISubsystem::ClearCachedObjects()
{
    CachedObjects.Empty();
}
