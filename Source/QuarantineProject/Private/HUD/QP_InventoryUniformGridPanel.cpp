// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/HUD/QP_InventoryUniformGridPanel.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/WidgetTree.h"

UQP_InventoryUniformGridPanel::UQP_InventoryUniformGridPanel(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{// set minimal inventory grid size
    GridSize = FIntPoint(1, 1);
	// Set the grid slot texture
	auto SlotTextureObj = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/HUD/InventoryHUD/Slot.Slot'"));
	if (SlotTextureObj.Object != nullptr)
	{
		SlotTexture = SlotTextureObj.Object;
	}
	// Set initial equip sound
	auto EquipSoundAsset = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("SoundWave'/Game/Sound/item-equip__1_.item-equip__1_'"));
	if (EquipSoundAsset.Object != nullptr)
	{
		EquipSound = EquipSoundAsset.Object;
	}
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
			if (SlotTexture != nullptr)
			{
				SlotImg->SetBrushFromTexture(SlotTexture);
			}
			SlotImg->SetOpacity(0.4f);
            // add slot background image
            GridPanel->AddChildToUniformGrid(SlotImg, row, col);
        }
    }
}

bool UQP_InventoryUniformGridPanel::AddItemToGrid(UQP_InventorySlotWidget* ItemWidget)
{
	// check if item's class is valid
	if (ItemWidget->InventoryItemInfo.ItemClassPtr)
	{
		// check if place in container exist
		auto FreeSlot = FindFreeSlotForItem(ItemWidget->InventoryItemInfo.SizeInInventory);
		if (FreeSlot.X != -1)
		{// set grid pointer to delete from grid after
			ItemWidget->CurrentGridOwner = this;
			// add item in container started from the found slot
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

bool UQP_InventoryUniformGridPanel::AddItemToGrid(UQP_InventorySlotWidget* ItemWidget, 
													const FIntPoint SlotPoint)
{
	// check if item's class is valid
	if (ItemWidget->InventoryItemInfo.ItemClassPtr)
	{
		// check if place in container exist
		bool bCanBePlacedInSlot = bIsItemCanBePlacedStartedFromSlot(SlotPoint, 
												ItemWidget->InventoryItemInfo.SizeInInventory);
		if (bCanBePlacedInSlot)
		{// set grid pointer to delete from grid after
			ItemWidget->CurrentGridOwner = this;
			// add item in container started from the found slot
			InsertItemInContainer(ItemWidget->InventoryItemInfo.SizeInInventory, SlotPoint, true);
			ItemWidget->InventoryItemInfo.PositionInInventory = SlotPoint;
			// add widget
			AddWidgetToGrid(ItemWidget, SlotPoint);
			return true;
		}
		else
		{// if current slot is not empty try to find another one
			if (AddItemToGrid(ItemWidget))
			{
				return true;
			}
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
			//
			OnInventoryGridChanged.Broadcast();
			// try and play the equip sound if specified
			if (EquipSound != NULL)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
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
				if (bIsItemCanBePlacedStartedFromSlot(FIntPoint(InternalColumn, InternalRow), ItemSize))
				{
					return FIntPoint(InternalColumn, InternalRow);
				}
			}
		}
	}
	return FIntPoint(-1, -1);
}

bool UQP_InventoryUniformGridPanel::bIsItemCanBePlacedStartedFromSlot(const FIntPoint FreeSlot,
																		const FIntPoint ItemSize) const
{
	auto Row = FreeSlot.Y;
	auto Column = FreeSlot.X;
	// check if the start pos + item size exceeds grid size
	if ((Column + ItemSize.X) > GridSize.X || (Row + ItemSize.Y) > GridSize.Y)
	{
		return false;
	}
	for (int32 SizeRow = Row; SizeRow < Row + ItemSize.Y; ++SizeRow)
	{
		for (int32 SizeCol = Column; SizeCol < Column + ItemSize.X; ++SizeCol)
		{
			if (Grid[SizeRow][SizeCol] == true)
			{// one of slots is not available
				return false;
			}
		}
	}
	// after iterating by all possible slots for that size -> all slots are empty return first available slot
	return true;
}

void UQP_InventoryUniformGridPanel::InsertItemInContainer(const FIntPoint ItemSize, const FIntPoint StartSlot, bool bIsItemInserted)
{
	for (int32 ItemCol = StartSlot.Y; ItemCol < StartSlot.Y + ItemSize.Y; ++ItemCol)
	{
		for (int32 ItemRow = StartSlot.X; ItemRow < StartSlot.X + ItemSize.X; ++ItemRow)
		{
			Grid[ItemCol][ItemRow] = bIsItemInserted;
		}
	}
}

TArray<UQP_InventorySlotWidget*> UQP_InventoryUniformGridPanel::GetItems()
{
	TArray<UQP_InventorySlotWidget*> Result;
	auto Slots = GridPanel->GetAllChildren();
	for (auto slot : Slots)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item in GRID: %s"), *slot->StaticClass()->GetName());
		if (Cast<UQP_InventorySlotWidget>(slot))
		{
			Result.Add(Cast<UQP_InventorySlotWidget>(slot));
		}
	}
	return Result;
}

void UQP_InventoryUniformGridPanel::ItemWasRemoved()
{
	OnInventoryGridChanged.Broadcast();
}

void UQP_InventoryUniformGridPanel::NativeOnDragEnter(const FGeometry& InGeometry, 
														const FDragDropEvent& InDragDropEvent, 
														UDragDropOperation* InOperation)
{
	if (GridPanel)
	{
		bIsItemDragged = true;
		if (bIsItemDragged)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item DRAGGED!"));
		}
		if (InOperation->Payload != nullptr)
		{
			auto const DraggedWidget = Cast<UQP_InventorySlotWidget>(InOperation->Payload);
			auto Geometry = InDragDropEvent.GetGestureDelta();
			UE_LOG(LogTemp, Warning, TEXT("Dragged widget gesture: %s"), *Geometry.ToString());
		}
	}
}

bool UQP_InventoryUniformGridPanel::NativeOnDrop(const FGeometry& InGeometry,
													const FDragDropEvent& InDragDropEvent,
															UDragDropOperation* InOperation)
{
	if (GridPanel)
	{
		bIsItemDragged = false;
		if (InOperation->Payload != nullptr)
		{
			auto const DraggedWidget = Cast<UQP_InventorySlotWidget>(InOperation->Payload);
			// find values to calculate drop position
			auto DropPos = InDragDropEvent.GetLastScreenSpacePosition();
			auto GridPanelPos = this->GetCachedGeometry().GetAbsolutePosition();
			auto GridPanelSize = this->GetCachedGeometry().GetAbsoluteSize();
			// find slot coordinates from grid position and size
			int32 Col = (DropPos.X - GridPanelPos.X) / (GridPanelSize.X / GridSize.X);
			int32 Row = (DropPos.Y - GridPanelPos.Y) / (GridPanelSize.Y / GridSize.Y);
			auto DropSlot = FIntPoint(Col, Row);

			if (AddItemToGrid(DraggedWidget, DropSlot))
			{
				return true;
			}
			else
			{//return to the parent grid if drop operation was not successful
				DraggedWidget->CurrentGridOwner->AddItemToGrid(DraggedWidget);
				return false;
			}
		}
		return true;
	}
	return true;
}

FReply UQP_InventoryUniformGridPanel::NativeOnMouseMove(const FGeometry& InGeometry, 
														const FPointerEvent& InMouseEvent)
{	
	auto DropPos = InMouseEvent.GetScreenSpacePosition();
	auto GridPanelPos = this->GetCachedGeometry().GetAbsolutePosition();
	auto GridPanelSize = this->GetCachedGeometry().GetAbsoluteSize();
	int32 Col = (DropPos.X - GridPanelPos.X) / (GridPanelSize.X / GridSize.X);
	int32 Row = (DropPos.Y - GridPanelPos.Y) / (GridPanelSize.Y / GridSize.Y);
	//
	UE_LOG(LogTemp, Warning, TEXT("Panel size: %s"), *GridPanelSize.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Panel size: %s"), *GridSize.ToString());
	UE_LOG(LogTemp, Warning, TEXT("POSITION FROM DRAG: %s, %s"), *DropPos.ToString(), *GridPanelPos.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Free slot FROM DRAG: %d, %d"), Col, Row);
	//
	auto FreeSlot = FIntPoint(Col, Row);
	auto SlotImage = GetGridPanelSlotAsImage(FreeSlot);
	//
	return FReply::Handled();
}

UImage* UQP_InventoryUniformGridPanel::GetGridPanelSlotAsImage(FIntPoint SlotPosition)
{
	auto Slots = GridPanel->GetAllChildren();
	int32 SlotIndex = SlotPosition.X + (SlotPosition.Y * GridSize.X);
	UE_LOG(LogTemp, Warning, TEXT("SlotPosition.X: %d"), SlotPosition.X);
	UE_LOG(LogTemp, Warning, TEXT("Slot to show: %d"), SlotPosition.Y);
	UE_LOG(LogTemp, Warning, TEXT("GridSize.X: %d"), GridSize.X);
	UE_LOG(LogTemp, Warning, TEXT("Slot to show: %d"), SlotIndex);
	if (Slots[SlotIndex])
	{
		auto SlotImage = Cast<UImage>(Slots[SlotIndex]);
		return SlotImage;
	}
	return nullptr;
}
