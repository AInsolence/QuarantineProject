// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuarantineProject/Public/Inventory/QP_InventoryItem.h"
#include "QP_InventorySlotWidget.generated.h"

/**
 * 
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUARANTINEPROJECT_API UQP_InventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UQP_InventorySlotWidget(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	// Events
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, 
									const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, 
										   const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickableItemProperties")
	FInventoryItemInfo InventoryItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickableItemProperties")
	// Set this if widget was dropped in grid
	class UQP_InventoryUniformGridPanel* CurrentGridOwner = nullptr;
	UFUNCTION(BlueprintCallable)
	FIntPoint GetItemSizeInInventory();
	UFUNCTION(BlueprintCallable)
	FIntPoint GetItemPositionInInventory();
	UFUNCTION(BlueprintCallable)
	UQP_InventorySlotWidget* SetInventoryItemSize(FIntPoint ItemSize);
	UPROPERTY(EditAnywhere, Category = "Settings")
	class USoundBase* PickUpSound = nullptr;

private:
	float BaseOpacity = 0.9f;
	float HoverOpacity = 1.f;

	// virtual void  	OnDragDetected
	// 							(
	// 							    FGeometry MyGeometry,
	// 							    const FPointerEvent & PointerEvent,
	// 							    UDragDropOperation *& Operation
	// 							) override;
	// TSubclassOf<class UMyDragDropOperation> DDOperClass;
};
