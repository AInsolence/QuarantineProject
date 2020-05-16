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

void UQP_InventoryWidget::AddSlotToGrid(UGridPanel* GridPanel, UWidget* Content, FIntPoint SlotPoint)
{
	if (GridPanel)
	{
		if (Content)
		{
			Content->SetVisibility(ESlateVisibility::Visible);
			auto Slots = GridPanel->GetSlots();
			int32 column = SlotPoint.X;
			int32 row = SlotPoint.Y;
			for (auto slot : Slots)
			{
				if (slot)
				{
					auto GridSlot = Cast<UGridSlot>(slot);
					if (GridSlot)
					{
						if (GridSlot->Layer > -1)
						{
							/*row = GridSlot->Row;
							column = GridSlot->Column + 1;*/
						}
					}
				}
					
			}
			GridPanel->AddChildToGrid(Content, row, column);
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Cannot add widget to slot"))
	}
}

void UQP_InventoryWidget::AddSlotToWeaponGrid(UWidget* Content, FIntPoint SlotPoint)
{
	AddSlotToGrid(WeaponGridPanel, Content, SlotPoint);
}

void UQP_InventoryWidget::AddSlotToBackPackGrid(UWidget* Content, FIntPoint SlotPoint)
{
	AddSlotToGrid(BackPackGridPanel, Content, SlotPoint);
}
