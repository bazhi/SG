
#include "SGEditorBlueprintLibrary.h"

#include "DataTableEditorUtils.h"

#include "SG/DataAsset/TableRowDefine.h"

bool USGEditorBlueprintLibrary::RenameRowsName(UDataTable* DataTable)
{
    if (DataTable)
    {
        DataTable->Modify();
        FName TempName = "TempName";
        TArray<FName> KeyNames = DataTable->GetRowNames();
        KeyNames.StableSort();
        for (int i = 0; i < KeyNames.Num(); ++i)
        {
            TempName.SetNumber(i);
            FDataTableEditorUtils::RenameRow(DataTable, KeyNames[i], TempName);
        }

        KeyNames = DataTable->GetRowNames();
        KeyNames.StableSort();
        for (int i = 0; i < KeyNames.Num(); ++i)
        {
            uint8* RowData = DataTable->GetRowMap().FindRef(KeyNames[i]);
            if (auto Row = reinterpret_cast<FDataTableRow*>(RowData))
            {
                FDataTableEditorUtils::RenameRow(DataTable, KeyNames[i], Row->GetRowName());
            }
        }
    }
    return true;
}

bool USGEditorBlueprintLibrary::SortDataTable(UDataTable* DataTable)
{
    if (DataTable)
    {
        DataTable->Modify();
        TArray<FName> KeyNames = DataTable->GetRowNames();
        KeyNames.StableSort();
        for (int i = 0; i < KeyNames.Num(); ++i)
        {
            FDataTableEditorUtils::MoveRow(DataTable, KeyNames[i], FDataTableEditorUtils::ERowMoveDirection::Down, KeyNames.Num());
        }
    }
    return true;
}
