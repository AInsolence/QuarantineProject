// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/HUD/QP_HUD.h"
#include "QuarantineProject/Public/HUD/AimingWidget.h"
#include "QuarantineProject/Public/HUD/QP_PlayerStateInfoWidget.h"
#include "QuarantineProject/Public/HUD/QP_PauseGameWidget.h"
#include "QuarantineProject/Public/HUD/QP_InventoryWidget.h"
#include "Kismet/GameplayStatics.h"

AQP_HUD::AQP_HUD(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	auto OpenInventorySoundAsset = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("SoundWave'/Game/Sound/open_inventory.open_inventory'"));
	if (OpenInventorySoundAsset.Object != nullptr)
	{
		OpenInventorySound = OpenInventorySoundAsset.Object;
	}
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
				PlayerStateInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else // show in the game
			{
				PlayerStateInfoWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	// add inventory widget
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UQP_InventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AQP_HUD::ShowInventory()
{
	if (InventoryWidget)
	{
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Hidden)
		{
			if (OpenInventorySound != NULL)
			{
				UGameplayStatics::PlaySound2D(this, OpenInventorySound);
			}
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
		else
		{
			if (OpenInventorySound != NULL)
			{
				UGameplayStatics::PlaySound2D(this, OpenInventorySound);
			}
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
	}
}

TArray<UQP_InventorySlotWidget*> AQP_HUD::GetWeaponGridItems()
{
	return InventoryWidget->GetWeaponGridItems();
}

bool AQP_HUD::AddSlotToWeaponGrid(UQP_InventorySlotWidget* ItemWidget) const
{
	if (InventoryWidget)
	{
		return InventoryWidget->AddSlotToWeaponGrid(ItemWidget);
	}
	return false;
}

bool AQP_HUD::AddSlotToBackPackGrid(UQP_InventorySlotWidget* ItemWidget) const
{
	if (InventoryWidget)
	{
		return InventoryWidget->AddSlotToBackPackGrid(ItemWidget);
	}
	return false;
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
