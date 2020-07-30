// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD/QP_InventorySlotWidget.h"
#include "QP_HUD.generated.h"

/**
 * 
 */

UCLASS()
class QUARANTINEPROJECT_API AQP_HUD : public AHUD
{
	GENERATED_BODY()

public:
    // Constructor
    AQP_HUD(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void DrawHUD() override;

    // Binding property in "meta" to prohibit compiling the HUD without appropriate widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UAimingWidget> AimingWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UQP_PlayerStateInfoWidget> PlayerStateInfoWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UQP_PauseGameWidget> PauseGameWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TSubclassOf<class UQP_InventoryWidget> InventoryWidgetClass;

    TArray<UUserWidget*> HUDWidgetsArray;// TODO make useful for storing all widgets

    UAimingWidget* AimingWidget;
    UQP_PlayerStateInfoWidget* PlayerStateInfoWidget;
    UQP_PauseGameWidget* PauseGameWidget;
    UQP_InventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, Category = "Settings")
	class USoundBase* OpenInventorySound = nullptr;

    /// *** API *** ///
    UFUNCTION()
    void SetCrosshairVisibility(bool bIsCrosshairVisible);
    UFUNCTION()
    void UpdateHealthState(float CurrentHealth);
    UFUNCTION()
    void UpdateStaminaState(float CurrentStamina);
    UFUNCTION()
    void SetPickUpTipVisibility(bool Show);
    UFUNCTION()
    void ShowInventory();
    UFUNCTION()
    TArray<UQP_InventorySlotWidget*> GetWeaponGridItems();
    UFUNCTION()
	bool AddSlotToWeaponGrid(UQP_InventorySlotWidget* ItemWidget) const;
    UFUNCTION()
	bool AddSlotToBackPackGrid(UQP_InventorySlotWidget* ItemWidget) const;
    UFUNCTION()
    void Exit();
    UFUNCTION()
    void GameOver();
};
