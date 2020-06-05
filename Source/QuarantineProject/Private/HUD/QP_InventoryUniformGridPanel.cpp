// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/HUD/QP_InventoryUniformGridPanel.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/WidgetTree.h"

UQP_InventoryUniformGridPanel::UQP_InventoryUniformGridPanel(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{// set minimal inventory grid size
    GridSize = FIntPoint(2, 2);
}

void UQP_InventoryUniformGridPanel::NativeConstruct()
{// fill grid with base slots
    for(int32 row = 0; row < GridSize.Y; ++row)
    {
        // add row to back-end grid
        Grid.Add(TArray<bool>());
        for (int32 col = 0; col < GridSize.X; ++col)
        {
            // add col value to back-end grid
            Grid[row].Add(false);
            // create slot background image
            auto SlotImg = NewObject<UImage>(UImage::StaticClass());
            SlotImg->SetBrushSize(FVector2D(128, 128));
            // add slot background image
            GridPanel->AddChildToUniformGrid(SlotImg, row, col);
        }
    }
}

bool UQP_InventoryUniformGridPanel::AddItemToGrid(UQP_InventorySlotWidget* ItemWidget)
{
	UE_LOG(LogTemp, Warning, TEXT("INSIDE add item to grid"));
	// check if item's class is valid
	if (ItemWidget->InventoryItemInfo.ItemClassPtr)
	{
		// check if place in container exist
		auto FreeSlot = FindFreeSlotForItem(ItemWidget->InventoryItemInfo.SizeInInventory);
		UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
		if (FreeSlot.X != -1)
		{// set grid pointer to delete from grid after
			ItemWidget->CurrentGridOwner = this;
			// add item in container started from the found slot
			UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
			InsertItemInContainer(ItemWidget->InventoryItemInfo.SizeInInventory, FreeSlot, true);
			ItemWidget->InventoryItemInfo.PositionInInventory = FreeSlot;
			// add widget
			AddWidgetToGrid(ItemWidget, FreeSlot);
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Not enough place in given container");
			return false;
		}
	}
	return false;
}

void UQP_InventoryUniformGridPanel::AddWidgetToGrid(UQP_InventorySlotWidget* ItemWidget, const FIntPoint SlotPoint)
{
    if (GridPanel)
    {
        auto Content = ItemWidget;
        if (Content)
        {
            Content->SetVisibility(ESlateVisibility::Visible);
            Content->SetRenderTransformPivot(FVector2D(0, 0));
            Content->SetRenderScale(FVector2D(ItemWidget->InventoryItemInfo.SizeInInventory.X,
                                              ItemWidget->InventoryItemInfo.SizeInInventory.Y));
			
            const int32 column = SlotPoint.X;
            const int32 row = SlotPoint.Y;
            auto InsertedItem = GridPanel->AddChildToUniformGrid(Content, row, column);
        }
    }
}

FIntPoint UQP_InventoryUniformGridPanel::FindFreeSlotForItem(const FIntPoint ItemSize) const
{
	// determine the possible search positions based on the size of the item
	int32 SearchRowSize = GridSize.Y - ItemSize.Y + 1;
	int32 SearchColumnSize = GridSize.X - ItemSize.X + 1;

	///*** try to find position for top left item slot ***///
	// iterate by rows in grid
	for (int32 InternalRow = 0; InternalRow < SearchRowSize; ++InternalRow)
	{// iterate by columns in row
		for (int32 InternalColumn = 0; InternalColumn < SearchColumnSize; ++InternalColumn)
		{// check if slot is free
			if (Grid[InternalRow][InternalColumn] == false)
			{/// *** try to find place for whole item ***///
				bool bIsNotPlaceForItem = false;
				for (int32 SizeRow = InternalRow; SizeRow < InternalRow + ItemSize.Y; ++SizeRow)
				{
					for (int32 SizeCol = InternalColumn; SizeCol < InternalColumn + ItemSize.X; ++SizeCol)
					{
						if (Grid[InternalRow][InternalColumn] == true)
						{// one of slots is not available
							bIsNotPlaceForItem = true;
							break;
						}
					}
					if (bIsNotPlaceForItem)
					{// one of slots is not available
						break;
					}
				}
				// after iterating by possible slots for that size -> all slots are empty
				if (!bIsNotPlaceForItem)
				{// then return first available slot
					return FIntPoint(InternalColumn, InternalRow);
				}
			}
		}
	}
	return FIntPoint(-1, -1);
}

void UQP_InventoryUniformGridPanel::InsertItemInContainer(const FIntPoint ItemSize, const FIntPoint StartSlot, bool bIsItemInserted)
{
	for (int32 ItemCol = StartSlot.Y; ItemCol < StartSlot.Y + ItemSize.Y; ++ItemCol)
	{
		for (int32 ItemRow = StartSlot.X; ItemRow < StartSlot.X + ItemSize.X; ++ItemRow)
		{
			if (bIsItemInserted)
			{
				UE_LOG(LogTemp, Warning, TEXT("INSERT TO SLOT %d, %d"), ItemCol, ItemRow);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DELETE FROM SLOT %d, %d"), ItemCol, ItemRow);
			}
			
			Grid[ItemCol][ItemRow] = bIsItemInserted;
		}
	}
}

bool UQP_InventoryUniformGridPanel::NativeOnDrop(const FGeometry& InGeometry,
													const FDragDropEvent& InDragDropEvent,
															UDragDropOperation* InOperation)
{
	if (GridPanel)
	{
		if (InOperation->Payload != nullptr)
		{
			auto const DraggedWidget = Cast<UQP_InventorySlotWidget>(InOperation->Payload);
			UE_LOG(LogTemp, Warning, TEXT("Try to drop item in Uniform BEFORE %d"), 
													DraggedWidget->InventoryItemInfo.SizeInInventory.X);
			// try to add item to grid
			if (AddItemToGrid(DraggedWidget))
			{
				UE_LOG(LogTemp, Warning, TEXT("Try to drop item in Uniform grid panel AFTER"));
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}
