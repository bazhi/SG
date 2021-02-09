#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SG/Interface/SGCameraInterface.h"

#include "SGPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class SG_API ASGPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    ASGPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
    void OnPossess(APawn* InPawn);

protected:
    virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

protected:
    void UpdateCameraBehavior(FVector& OutLocation, FRotator& OutRotation, float& OutFOV);

    FVector CalculateAxisIndependentLag(const FVector& CurrentLocation, const FVector& TargetLocation, const FRotator& CameraRotation, const FVector& LagSpeeds);
    float GetCurveValue(FName CurveName) const;
    EDrawDebugTrace::Type GetDebugTraceType(EDrawDebugTrace::Type ShowTraceType);
private:
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* CameraMesh;

protected:
    TWeakObjectPtr<APawn> ControllerdPawn = nullptr;
    UPROPERTY(Transient)
    TScriptInterface<ISGCameraInterface> ICamera = nullptr;

    FVector RootLocation;
    FTransform SmoothedPivotTarget;
    FVector PivotLocation;
    FVector TargetCameraLocation;
    FVector TargetCameraRotation;

    FVector DebugViewOffset;
    FRotator DebugViewRotation;
};
