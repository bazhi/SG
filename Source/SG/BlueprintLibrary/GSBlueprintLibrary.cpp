#include "GSBlueprintLibrary.h"

#include "ScopedTransaction.h"

#include "Kismet/KismetMathLibrary.h"
#include "SG/Character/ECharacter.h"
#include "SG/DataAsset/TableRowDefine.h"

#if WITH_EDITOR
#include "DataTableEditorUtils.h"
#endif

void UGSBlueprintLibrary::ConvertWorldToLocal(const FGSComponentAndTransform& WorldSpaceComponent, FGSComponentAndTransform& LocalSpaceComponent)
{
    LocalSpaceComponent.Component = WorldSpaceComponent.Component;
    LocalSpaceComponent.Transform = WorldSpaceComponent.Transform * (UKismetMathLibrary::InvertTransform(WorldSpaceComponent.Component->GetComponentToWorld()));
}

void UGSBlueprintLibrary::ConvertLocalToWorld(const FGSComponentAndTransform& LocalSpaceComponent, FGSComponentAndTransform& WorldSpaceComponent)
{
    WorldSpaceComponent.Component = LocalSpaceComponent.Component;
    WorldSpaceComponent.Transform = LocalSpaceComponent.Transform * LocalSpaceComponent.Component->GetComponentToWorld();
}

FTransform UGSBlueprintLibrary::TransformSubtraction(const FTransform& A, const FTransform B)
{
    FTransform Result;
    Result.SetLocation(A.GetLocation() - B.GetLocation());
    Result.SetRotation((A.Rotator() - B.Rotator()).Quaternion());
    Result.SetScale3D(A.GetScale3D() - B.GetScale3D());
    return Result;
}

FTransform UGSBlueprintLibrary::TransformAddition(const FTransform& A, const FTransform B)
{
    FTransform Result;
    Result.SetLocation(A.GetLocation() + B.GetLocation());
    Result.SetRotation((A.Rotator() + B.Rotator()).Quaternion());
    Result.SetScale3D(A.GetScale3D() + B.GetScale3D());
    return Result;
}

bool UGSBlueprintLibrary::RenameRowsName(UDataTable* DataTable)
{
#if WITH_EDITOR
    if (DataTable)
    {
        TMap<FName, uint8*> RowMap = DataTable->GetRowMap();
        DataTable->Modify();
        RowMap = DataTable->GetRowMap();
        for (TMap<FName, uint8*>::TConstIterator RowMapIter(RowMap); RowMapIter; ++RowMapIter)
        {
            FTableRowBase* Entry = reinterpret_cast<FTableRowBase*>(RowMapIter.Value());
            if (auto Row = static_cast<FDataTableRow*>(Entry))
            {
                FName NewName = Row->GetRowName();
                if (!DataTable->GetRowMap().Find(NewName))
                {
                    DataTable->RemoveRow(RowMapIter.Key());
                    DataTable->AddRow(NewName, *Row);
                }
            }
        }
    }
#endif
    return true;
}

bool UGSBlueprintLibrary::RenameRowsNameTemp(UDataTable* DataTable)
{
#if WITH_EDITOR
    if (DataTable)
    {
        TMap<FName, uint8*> RowMap = DataTable->GetRowMap();
        DataTable->Modify();
        int i = 1;
        FName TempName = "Temp";
        for (TMap<FName, uint8*>::TConstIterator RowMapIter(RowMap); RowMapIter; ++RowMapIter)
        {
            FTableRowBase* Entry = reinterpret_cast<FTableRowBase*>(RowMapIter.Value());
            if (auto Row = static_cast<FDataTableRow*>(Entry))
            {
                FName NewName = TempName;
                NewName.SetNumber(i);
                ++i;
                if (!DataTable->GetRowMap().Find(NewName))
                {
                    DataTable->RemoveRow(RowMapIter.Key());
                    DataTable->AddRow(NewName, *Row);
                }
            }
        }
    }
#endif
    return true;
}
