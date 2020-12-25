

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SGPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class SG_API ASGPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	void OnPossess(APawn* InPawn);

protected:
	TWeakObjectPtr<APawn> ControllerdPawn;
};
