

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ConfigGISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UConfigGISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	virtual void Deinitialize();
};
