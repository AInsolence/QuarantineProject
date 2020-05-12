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

void UQP_InventoryWidget::AddSlotToWeaponGrid(UWidget* Content, int32 InRow, int32 InColumn)
{
	if (WeaponGridPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Try to add slot"))
		if (Content)
		{
			Content->SetVisibility(ESlateVisibility::Visible);
			auto Slots = WeaponGridPanel->GetSlots();
			int32 row = 0;
			int32 column = 0;
			for (auto WeaponSlot : Slots)
			{
				UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Slots start"))
				if (WeaponSlot)
				{
					UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Slot in Slots"))
					auto WeaponGridSlot = Cast<UGridSlot>(WeaponSlot);
					if (WeaponGridSlot)
					{
						UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Cast to grid"))
						if (WeaponGridSlot->Layer > -1)
						{
							UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Set row and col"))
							row = WeaponGridSlot->Row;
							column = WeaponGridSlot->Column + 1;
						}
					}
				}
					
			}
			WeaponGridPanel->AddChildToGrid(Content, row, column);
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QP_InventoryWidget: Cannot add widget to slot"))
	}
}
