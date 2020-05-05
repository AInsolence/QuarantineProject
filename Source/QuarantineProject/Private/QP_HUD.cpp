// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_HUD.h"
#include "AimingWidget.h"
#include "QP_PlayerStateInfoWidget.h"
#include "QP_PauseGameWidget.h"
#include "Kismet/GameplayStatics.h"

AQP_HUD::AQP_HUD(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	
}

void AQP_HUD::BeginPlay()
{
	Super::BeginPlay();
	//*** NOTE ZOrder for created widgets depends on their order in this code ***//
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
	// add pause widget
	if (PauseGameWidgetClass)
	{
		PauseGameWidget = CreateWidget<UQP_PauseGameWidget>(GetWorld(), PauseGameWidgetClass);
		if (PauseGameWidget)
		{
			PauseGameWidget->AddToViewport();
			PauseGameWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AQP_HUD::SetPickUpTipVisibility(bool Show)
{
	if (PlayerStateInfoWidget)
	{
		PlayerStateInfoWidget->SetPickUpTipVisibility(Show);
	}
}

void AQP_HUD::Exit()
{
	if (PauseGameWidget)
	{
		if (PauseGameWidget->Visibility == ESlateVisibility::Hidden)
		{
			PauseGameWidget->SetVisibility(ESlateVisibility::Visible);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
		else
		{
			PauseGameWidget->SetVisibility(ESlateVisibility::Hidden);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
	}
}

void AQP_HUD::GameOver()
{
	if (PauseGameWidget)
	{
		if (!GetWorld()) return;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		PauseGameWidget->SetVisibility(ESlateVisibility::Visible);
		PauseGameWidget->ShowGameOverText();
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}
