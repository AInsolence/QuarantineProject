// Fill out your copyright notice in the Description page of Project Settings.

#include "..\..\Public\HUD\QP_InventoryWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"

UQP_InventoryWidget::UQP_InventoryWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	
}

void UQP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryHeader)
	{
		InventoryHeader->SetVisibility(ESlateVisibility::Visible);

		//ExitButton->OnClicked.AddDynamic(this, &UQP_PauseGameWidget::ExitGame);
	}
}

void UQP_InventoryWidget::AddSlotToGrid(UUniformGridPanel* GridPanel, FInventoryItemInfo ItemInfo, FIntPoint SlotPoint)
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
			
			int32 column = SlotPoint.X;
			int32 row = SlotPoint.Y;
			auto InsertedItem = GridPanel->AddChildToUniformGrid(Content, row, column);
			
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Cannot add widget to slot"))
	}
}

void UQP_InventoryWidget::AddSlotToWeaponGrid(FInventoryItemInfo ItemInfo, FIntPoint SlotPoint)
{
	AddSlotToGrid(WeaponGridPanel, ItemInfo, SlotPoint);
}

void UQP_InventoryWidget::AddSlotToBackPackGrid(FInventoryItemInfo ItemInfo, FIntPoint SlotPoint)
{
	AddSlotToGrid(BackPackGridPanel, ItemInfo, SlotPoint);
}
