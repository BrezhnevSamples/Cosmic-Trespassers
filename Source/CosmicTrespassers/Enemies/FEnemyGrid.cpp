// Copyright Alexander Terentyev. All Rights Reserved.

#include "FEnemyGrid.h"

FEnemyGrid::FEnemyGrid(const FEnemyGridSettings& GridSettings): Cells(0, 0), LeftmostColumnIndex(0), RightmostColumnIndex(0)
{
	const uint8 ColumnCount = GridSettings.ColumnCount;
	const uint8 RowCount = GridSettings.RowCount;
	const float DistanceBetweenColumns = GridSettings.DistanceBetweenColumns;
	const float DistanceBetweenRows = GridSettings.DistanceBetweenRows;

	const float GridWidth = (ColumnCount - 1) * DistanceBetweenColumns;

	const float StartY = -GridWidth * 0.5;

	Cells = TArray2D<FEnemyGridCell>(ColumnCount, RowCount);

	const float StartZ = GridSettings.GridStartZ;

	for (uint8 ColumnIndex = 0; ColumnIndex < ColumnCount; ColumnIndex++)
	{
		for (uint8 RowIndex = 0; RowIndex < RowCount; RowIndex++)
		{
			const FVector CellLocation = FVector(0, StartY + ColumnIndex * DistanceBetweenColumns,
			                                     StartZ + RowIndex * DistanceBetweenRows);
			const auto Cell = FEnemyGridCell(FVector2Uint8(ColumnIndex + 1, RowIndex + 1), CellLocation);
			Cells.Set(Cell, ColumnIndex, RowIndex);
		}
	}
}
