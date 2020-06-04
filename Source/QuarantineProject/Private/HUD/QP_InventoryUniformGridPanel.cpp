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
        for (int32 col = 0; col < GridSize.X; ++col)
        {
            auto SlotImg = NewObject<UImage>(UImage::StaticClass());
            SlotImg->SetBrushSize(FVector2D(128, 128));
            GridPanel->AddChildToUniformGrid(SlotImg, row, col);
        }
    }
}

void UQP_InventoryUniformGridPanel::AddSlotToGrid(FInventoryItemInfo ItemInfo, FIntPoint SlotPoint)
{
    if (GridPanel)
    {
        auto Content = ItemInfo.InventorySlotWidget;
        if (Content)
        {
            Content->SetVisibility(ESlateVisibility::Visible);
            Content->SetRenderTransformPivot(FVector2D(0, 0));
            Content->SetRenderScale(FVector2D(ItemInfo.SizeInInventory.X,
                                              ItemInfo.SizeInInventory.Y));
			
            const int32 column = SlotPoint.X;
            const int32 row = SlotPoint.Y;
            auto InsertedItem = GridPanel->AddChildToUniformGrid(Content, row, column);
        }
    }
}

bool UQP_InventoryUniformGridPanel::NativeOnDrop(const FGeometry& InGeometry,
                                                 const FDragDropEvent& InDragDropEvent,
                                                 UDragDropOperation* InOperation)
{
    if(GridPanel)
    {
        if (InOperation->Payload != nullptr)
        {
            auto const DraggedWidget = Cast<UWidget>(InOperation->Payload);
            GridPanel->AddChild(DraggedWidget);
			return true;
        }
        return false;
    }
    return false;
}
