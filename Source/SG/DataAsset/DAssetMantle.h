#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAssetMantle.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UDAssetMantle : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    class UAnimMontage* AnimMontage = nullptr;
    UPROPERTY(EditDefaultsOnly)
    class UCurveVector* PositionCorrectionCurve = nullptr;
    UPROPERTY(EditDefaultsOnly)
    FVector StartingOffset = FVector::ZeroVector;
    UPROPERTY(EditDefaultsOnly)
    float LowHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float LowPlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float LowStartPosition = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighHeight = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighPlayRate = 0;
    UPROPERTY(EditDefaultsOnly)
    float HighStartPosition = 0;
};
