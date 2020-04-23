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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Blood;
	UFUNCTION()
	void UpdateHealthState(float CurrentHealth);
	UFUNCTION()
	void UpdateStaminaState(float CurrentStamina);

private:
	void StoreWidgetAnimations();
	UWidgetAnimation* const GetAnimationByName(FName AnimationName) const;
	TMap<FName, UWidgetAnimation*> AnimationsMap;

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* HealthBarAnimation = nullptr;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* StaminaBarAnimation = nullptr;
};
