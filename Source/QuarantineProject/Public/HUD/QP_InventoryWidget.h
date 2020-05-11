// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	class UGridPanel* WeaponGridPanel = nullptr;

	void AddSlotToWeaponGrid(UWidget* Content, int32 InRow = 0, int32 InColumn = 0);
};
