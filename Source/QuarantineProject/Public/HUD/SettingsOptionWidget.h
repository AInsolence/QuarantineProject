// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOptionWidget.generated.h"

/**
	This class represents a settings menu option block. 
	Used to create a new option for settings menu via Blueprint.
 */

UCLASS(Config=GameUserSettings)
class QUARANTINEPROJECT_API USettingsOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Constructor
	USettingsOptionWidget(const FObjectInitializer&);
	// NativeConstruct
	virtual void NativeConstruct();
	// Components
	UPROPERTY(BlueprintReadWrite, Category = "Settings button", meta = (BindWidget))
	class UTextBlock* OptionName;
	UPROPERTY(BlueprintReadWrite, Category = "Settings button", meta = (BindWidget))
	class UComboBoxString* OptionValues;
	// Init methods
	void SetName(const FString Name);
	void SetValues(const TMap<FString, FString> Values);
	void SetValue(const FString Value);
	// Mutable variables are set by developer in BP
	UPROPERTY(BlueprintReadOnly, Category = "Settings button")
	FString ConsoleCommandPrefix;
	UPROPERTY(BlueprintReadOnly, Category = "Settings button")
	TMap<FString, FString> ConsoleCommandsMap;
	UFUNCTION(BlueprintCallable)
	void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
