#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SG/Character/ECharacter.h"
#include "UObject/NoExportTypes.h"
#include "TableRowDefine.generated.h"

const FName RowNameNull = "";

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
        Result.SetNumber(ID);
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
        Result.SetNumber((ID32 << 8) + SubID);
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
        Result.SetNumber((uint8)ID);
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
};
