// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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
    // Binding property in "meta" to prohibit compiling the HUD without widget
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UAimingWidget> AimingWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UQP_PlayerStateInfoWidget> PlayerStateInfoWidgetClass;

    TArray<UUserWidget*> HUDWidgetsArray;// TODO make useful for storing all widgets

    UAimingWidget* AimingWidget;
    UQP_PlayerStateInfoWidget* PlayerStateInfoWidget;

    UFUNCTION()
    void SetCrosshairVisibility(bool bIsCrosshairVisible);
};
