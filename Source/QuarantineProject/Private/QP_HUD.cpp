// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_HUD.h"
#include "AimingWidget.h"
#include "QP_PlayerStateInfoWidget.h"

AQP_HUD::AQP_HUD(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

void AQP_HUD::BeginPlay()
{
	Super::BeginPlay();
	// add aiming widget
	if (AimingWidgetClass)
	{
		AimingWidget = CreateWidget<UAimingWidget>(GetWorld(), AimingWidgetClass);
		if (AimingWidget)
		{
			AimingWidget->AddToViewport();
			AimingWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	// add player state  widget
	if (PlayerStateInfoWidgetClass)
	{
		PlayerStateInfoWidget = CreateWidget<UQP_PlayerStateInfoWidget>(GetWorld(), PlayerStateInfoWidgetClass);
		if (PlayerStateInfoWidget)
		{
			PlayerStateInfoWidget->AddToViewport();
			// Hide player status widget in main menu
			auto LevelName = GetWorld()->GetName();
			if (LevelName == "MainMenu")
			{
				UE_LOG(LogTemp, Warning, TEXT("Try to hid %s"), *LevelName)
					PlayerStateInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else // show in the game
			{
				PlayerStateInfoWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void AQP_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto LevelName = GetWorld()->GetName();
}

void AQP_HUD::DrawHUD()
{
	Super::DrawHUD();
}

void AQP_HUD::SetCrosshairVisibility(bool bIsCrosshairVisible)
{
	if (bIsCrosshairVisible)
	{
		AimingWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AimingWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AQP_HUD::UpdateHealthState(float CurrentHealth)
{
	if (PlayerStateInfoWidget)
	{
		PlayerStateInfoWidget->UpdateHealthState(CurrentHealth);
	}
}

void AQP_HUD::UpdateStaminaState(float CurrentStamina)
{
	if (PlayerStateInfoWidget)
	{
		PlayerStateInfoWidget->UpdateStaminaState(CurrentStamina);
	}
}
