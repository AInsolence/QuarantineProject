// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QP_PauseGameWidget.generated.h"

/**
 * 
 */

UCLASS()
class QUARANTINEPROJECT_API UQP_PauseGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Constructor
	UQP_PauseGameWidget(const FObjectInitializer& ObjectInitializer);
	// Native constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ResumeButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void ExitGame();
};
