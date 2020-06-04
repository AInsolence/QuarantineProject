// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuarantineProject/Public/HUD/QP_InventorySlotWidget.h"
#include "QP_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUARANTINEPROJECT_API UQP_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Constructor
	UQP_InventoryWidget(const FObjectInitializer& ObjectInitializer);
	// Native constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* InventoryHeader = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UQP_InventoryUniformGridPanel* WeaponGridPanel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UQP_InventoryUniformGridPanel* BackPackGridPanel = nullptr;

	void AddSlotToWeaponGrid(UQP_InventorySlotWidget* ItemWidget, FIntPoint SlotPoint) const;
	void AddSlotToBackPackGrid(UQP_InventorySlotWidget* ItemWidget, FIntPoint SlotPoint) const;
};
