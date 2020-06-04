// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UniformGridPanel.h"
#include "Blueprint/UserWidget.h"
#include "QuarantineProject/Public/Inventory/QP_InventoryItem.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UUniformGridPanel* GridPanel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint GridSize;
	void AddSlotToGrid(FInventoryItemInfo ItemInfo, FIntPoint SlotPoint);

	virtual bool NativeOnDrop(const FGeometry & InGeometry,
                              const FDragDropEvent & InDragDropEvent,
                              UDragDropOperation * InOperation) override;
};
