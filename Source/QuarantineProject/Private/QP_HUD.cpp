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
	if (AimingWidgetClass)
	{
		AimingWidget = CreateWidget<UAimingWidget>(GetWorld(), AimingWidgetClass);
		if (AimingWidget)
		{
			AimingWidget->AddToViewport();
		}
	}
	if (PlayerStateInfoWidgetClass)
	{
		PlayerStateInfoWidget = CreateWidget<UQP_PlayerStateInfoWidget>(GetWorld(), PlayerStateInfoWidgetClass);
		if (PlayerStateInfoWidget)
		{
			PlayerStateInfoWidget->AddToViewport();
		}
	}
}

void AQP_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
