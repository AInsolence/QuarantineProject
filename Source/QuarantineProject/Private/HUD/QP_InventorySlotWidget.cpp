// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QP_InventorySlotWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/Image.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "HUD/MyDragDropOperation.h"

UQP_InventorySlotWidget::UQP_InventorySlotWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	auto PickUpSoundAsset = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("SoundWave'/Game/Sound/pickup.pickup'"));
	if (PickUpSoundAsset.Object != nullptr)
	{
		PickUpSound = PickUpSoundAsset.Object;
	}
}

void UQP_InventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemImage)
	{
		ItemImage->SetOpacity(BaseOpacity);
	}
}

void UQP_InventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, 
                                                 const FPointerEvent& InMouseEvent)
{
	if (ItemImage)
	{
		ItemImage->SetOpacity(HoverOpacity);
	}
}

void UQP_InventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (ItemImage)
	{
		ItemImage->SetOpacity(BaseOpacity);
	}
}

FReply UQP_InventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, 
														const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Inside NativeOnMouseDown"))
	auto Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,
											this, FKey("LeftMouseButton"));
	if (PickUpSound != NULL)
	{
		UGameplayStatics::PlaySound2D(this, PickUpSound);
	}
	return Reply.NativeReply;
}

FIntPoint UQP_InventorySlotWidget::GetItemSizeInInventory()
{
	return InventoryItemInfo.SizeInInventory;
}

FIntPoint UQP_InventorySlotWidget::GetItemPositionInInventory()
{
	return InventoryItemInfo.PositionInInventory;
}

UQP_InventorySlotWidget* UQP_InventorySlotWidget::SetInventoryItemSize(FIntPoint ItemSize)
{
	InventoryItemInfo.SizeInInventory = ItemSize;
	return this;
}

// void UQP_InventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, 
// 												   const FPointerEvent& InMouseEvent, 
// 												   UDragDropOperation*& OutOperation)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Inside NativeOnDragDetected"))
// 	auto const DnDOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DDOperClass);
// 	DnDOperation->DefaultDragVisual = CreateWidget(this, this->GetClass(), "DragWidget");
// 	if (DnDOperation != nullptr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Inside NativeOnDragDetected DDOperation exist"))
// 	}
// }
