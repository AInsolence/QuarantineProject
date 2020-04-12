// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_PlayerStateInfoWidget.h"

#include "Runtime/UMG/Public/UMG.h"

UQP_PlayerStateInfoWidget::UQP_PlayerStateInfoWidget(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
}

void UQP_PlayerStateInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HealthBar && StaminaBar)
    {// show if hidden
        if (HealthBar->Visibility == ESlateVisibility::Hidden)
        {
            HealthBar->SetVisibility(ESlateVisibility::Visible);
        }
        if (StaminaBar->Visibility == ESlateVisibility::Hidden)
        {
            StaminaBar->SetVisibility(ESlateVisibility::Visible);
        }
    }
}