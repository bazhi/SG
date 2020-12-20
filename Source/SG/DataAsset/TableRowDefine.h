#pragma once

#include "CoreMinimal.h"
#include "DAssetMantle.h"

#include "Engine/DataTable.h"
#include "SG/Character/ECharacter.h"
#include "UObject/NoExportTypes.h"
#include "TableRowDefine.generated.h"

const FName RowNameNull = "";

const uint8 NameBaseNumber = 1;

USTRUCT()
struct FDataTableRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const
    {
        return RowNameNull;
    }
};

USTRUCT()
struct FDataTableRowOne : public FDataTableRow
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const override
    {
        FName Result = "K";
        Result.SetNumber(ID + NameBaseNumber);
        return Result;
    }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    uint32 ID = 0;
};

USTRUCT()
struct FDataTableRowTwo : public FDataTableRow
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const override
    {
        FName Result = "K";
        uint32 ID32 = ID;
        Result.SetNumber((ID32 << 8) + SubID + NameBaseNumber);
        return Result;
    }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    uint16 ID = 0;
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    uint8 SubID = 0;
};

USTRUCT()
struct FDTRowOverlayState : public FDataTableRow
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const override
    {
        FName Result = "K";
        Result.SetNumber((uint8)ID + NameBaseNumber);
        return Result;
    }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    EOverlayState ID = EOverlayState::Default;

    UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
    TSoftObjectPtr<UAnimMontage> AnimMontageRoll;
    UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
    TSoftObjectPtr<UAnimMontage> AnimMontageFrontUp;
    UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
    TSoftObjectPtr<UAnimMontage> AnimMontageBackUp;

    UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
    TSoftObjectPtr<UDAssetMantle> AssetMantleDefault;
    UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
    TMap<EMantleType, TSoftObjectPtr<UDAssetMantle>> AssetMantleMap;
};

USTRUCT()
struct FDTRowMovementSetting : public FDataTableRow
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const override
    {
        FName Result = "K";
        uint32 ID32 = (uint32)RotationMode;
        Result.SetNumber((ID32 << 8) + (uint8)Stance + NameBaseNumber);
        return Result;
    }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    ERotationMode RotationMode = ERotationMode::VelocityDirection;
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    EStance Stance = EStance::Standing;

public:
    UPROPERTY(EditDefaultsOnly)
    float WalkSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    float RunSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    float SprintSpeed = 0;
    UPROPERTY(EditDefaultsOnly)
    class UCurveVector* MovementCurve = nullptr;
    UPROPERTY(EditDefaultsOnly)
    class UCurveFloat* RotationRateCurve = nullptr;
};

USTRUCT()
struct FDTRowTurnInPlace : public FDataTableRow
{
    GENERATED_BODY()

public:
    virtual FName GetRowName() const override
    {
        FName Result = "K";
        uint32 ID32 = (uint32)TurnMode;
        Result.SetNumber(ID32 + NameBaseNumber);
        return Result;
    }

public:
    UPROPERTY(EditDefaultsOnly, Category = "Base ID")
    ETurnInPlace TurnMode = ETurnInPlace::NL90;

public:
    UPROPERTY(EditDefaultsOnly)
    class UAnimSequenceBase* Animation = nullptr;
    UPROPERTY(EditDefaultsOnly)
    float AnimatedAngle = 0;
    UPROPERTY(EditDefaultsOnly)
    FName SlotName = "None";
    UPROPERTY(EditDefaultsOnly)
    float PlayRate = 1.0f;
    UPROPERTY(EditDefaultsOnly)
    bool ScaleTurnAngle = true;
};
