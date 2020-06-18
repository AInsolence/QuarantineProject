// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UniformGridPanel.h"
#include "Blueprint/UserWidget.h"
#include "QuarantineProject/Public/HUD/QP_InventorySlotWidget.h"
#include "Containers/Array.h"
#include "QP_InventoryUniformGridPanel.generated.h"

/**
 * 
 */
UCLASS()
class QUARANTINEPROJECT_API UQP_InventoryUniformGridPanel : public UUserWidget
{
	GENERATED_BODY()
	public:
    // Constructor
    UQP_InventoryUniformGridPanel(const FObjectInitializer& ObjectInitializer);
	// Native constructor
	virtual void NativeConstruct() override;
	// UI representation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UUniformGridPanel* GridPanel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint GridSize;
	// Back-end grid representation
	TArray<TArray<bool>> Grid;

	virtual void NativeOnDragEnter( const FGeometry& InGeometry, 
									const FDragDropEvent& InDragDropEvent, 
									UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry & InGeometry,
                              const FDragDropEvent & InDragDropEvent,
                              UDragDropOperation * InOperation) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, 
										const FPointerEvent& InMouseEvent) override;

	// Add an item to the grid back-end + widget
	bool AddItemToGrid(UQP_InventorySlotWidget* ItemWidget);
	bool AddItemToGrid(UQP_InventorySlotWidget* ItemWidget, const FIntPoint SlotPoint);
	// Add an item widget to the grid panel widget
	void AddWidgetToGrid(UQP_InventorySlotWidget* ItemWidget, const FIntPoint SlotPoint);
	// Returns free slot coordinates for given item size or FIntPoint(-1, -1)
	FIntPoint FindFreeSlotForItem(const FIntPoint ItemSize) const;
	bool bIsItemCanBePlacedStartedFromSlot(const FIntPoint FreeSlot, const FIntPoint ItemSize) const;
	UFUNCTION(BlueprintCallable)
	// Insert the item to the back-end grid to the given position if bIsItemInserted = true or delete if false
	void InsertItemInContainer(const FIntPoint ItemSize, const FIntPoint StartSlot, bool bIsItemInserted);

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	UTexture2D* SlotTexture;
	UPROPERTY(EditAnywhere, Category = "Settings")
	class USoundBase* EquipSound = nullptr;

	bool bIsItemDragged = false;
	UImage* GetGridPanelSlotAsImage(FIntPoint SlotPosition);
};
