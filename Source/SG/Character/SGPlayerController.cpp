#include "SGPlayerController.h"

#include "SGPlayerCameraManager.h"


#include "Blueprint/WidgetBlueprintLibrary.h"

void ASGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(HUD)
	{
		WidgetHUD = UWidgetBlueprintLibrary::Create(this, HUD, this);
		if(WidgetHUD)
		{
			WidgetHUD->AddToViewport();
		}
	}
}

void ASGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ASGPlayerCameraManager* CameraManager = Cast<ASGPlayerCameraManager>(PlayerCameraManager))
	{
		CameraManager->OnPossess(InPawn);
	}
}
