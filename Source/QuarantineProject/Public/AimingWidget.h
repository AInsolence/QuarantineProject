// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimingWidget.generated.h"

/**
 * 
 */

UCLASS()
class QUARANTINEPROJECT_API UAimingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Constructor
	UAimingWidget(const FObjectInitializer& ObjectInitializer);
	// Native constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* AimModeCrosshair;
};
