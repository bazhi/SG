#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SG_API ASGPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UserWidget")
    TSubclassOf<UUserWidget> HUD;
    UPROPERTY(EditDefaultsOnly, Category = "UserWidget")
    TSubclassOf<UUserWidget> OverlayState;

protected:
    UPROPERTY(Transient)
    UUserWidget* WidgetHUD;
    UPROPERTY(Transient)
    UUserWidget* OverlaySwitcher;
protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
};
