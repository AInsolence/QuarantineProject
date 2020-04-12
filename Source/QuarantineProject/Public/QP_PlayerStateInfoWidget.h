// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QP_PlayerStateInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUARANTINEPROJECT_API UQP_PlayerStateInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Constructor
	UQP_PlayerStateInfoWidget(const FObjectInitializer& ObjectInitializer);
	// Native constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;
};
