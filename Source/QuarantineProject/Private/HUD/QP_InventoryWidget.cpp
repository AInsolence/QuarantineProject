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
		//auto GridSlot = Cast<UGridSlot>(Content);
		//GridSlot->SetRowSpan(2);
		//GridSlot->SetColumnSpan(4);
		if (Content)
		{
			Content->SetVisibility(ESlateVisibility::Visible);
			WeaponGridPanel->AddChildToGrid(Content, InRow, InColumn);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,  TEXT("QP_InventoryWidget: Cannot add widget to slot"))
	}
}
