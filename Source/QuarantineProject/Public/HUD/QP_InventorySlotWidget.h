// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QP_InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
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
	// virtual void  	OnDragDetected
	// 							(
	// 							    FGeometry MyGeometry,
	// 							    const FPointerEvent & PointerEvent,
	// 							    UDragDropOperation *& Operation
	// 							) override;
	// TSubclassOf<class UMyDragDropOperation> DDOperClass;
private:
	float BaseOpacity = 0.9f;
	float HoverOpacity = 1.f;
};
