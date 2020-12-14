


#include "ConfigGISubsystem.h"


#include "SG/DataAsset/ConfigManager.h"
#include "SG/GameInstance/SGGameInstance.h"

void UConfigGISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if(USGGameInstance* GSGameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
        if(GSGameInstance->GetConfigManager())
        {
            GSGameInstance->GetConfigManager()->OnStartupLoad();
        }
	}
}

void UConfigGISubsystem::Deinitialize()
{
}
