// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UAimingWidget::UAimingWidget(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
}

void UAimingWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (AimModeCrosshair)
    {// show if hidden
        if (AimModeCrosshair->Visibility == ESlateVisibility::Hidden)
        {
            AimModeCrosshair->SetVisibility(ESlateVisibility::Visible);
        }
    }
}
