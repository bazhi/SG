#include "ConfigGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SG/DataAsset/ConfigManager.h"
#include "SG/GameInstance/SGGameInstance.h"

void UConfigGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
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

void UConfigGameSubsystem::Deinitialize()
{

}

UConfigGameSubsystem* UConfigGameSubsystem::Get(UGameInstance* GameInstance)
{
	if(GameInstance)
	{
		return GameInstance->GetSubsystem<UConfigGameSubsystem>();
	}
	return nullptr;
}

