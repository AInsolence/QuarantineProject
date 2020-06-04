// Fill out your copyright notice in the Description page of Project Settings.

#include "..\..\Public\HUD\QP_InventoryWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"
#include "QuarantineProject/Public/HUD/QP_InventoryUniformGridPanel.h"

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
	}
}

void UQP_InventoryWidget::AddSlotToWeaponGrid(UQP_InventorySlotWidget* ItemWidget, FIntPoint SlotPoint) const
{
	WeaponGridPanel->AddSlotToGrid(ItemWidget, SlotPoint);
}

void UQP_InventoryWidget::AddSlotToBackPackGrid(UQP_InventorySlotWidget* ItemWidget, FIntPoint SlotPoint) const
{
	BackPackGridPanel->AddSlotToGrid(ItemWidget, SlotPoint);
}
