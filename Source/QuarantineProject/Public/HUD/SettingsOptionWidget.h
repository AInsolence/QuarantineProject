// Copyright © 2020 Insolence Assets, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOptionWidget.generated.h"

/**
	This class represents a settings menu option block.
	Used to create a new option for settings menu via Blueprint.
 */

UCLASS()
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

	// Public methods
	void SetName(const FString Name); // option name
	void SetValue(const FString Value);
	void SetValues(const TMap<FString, FString> Values); // set values/console commands map

	// Mutable variables are set by developer in BP
	UPROPERTY(BlueprintReadOnly, Category = "Settings button")
		FString ConsoleCommandPrefix;
	UPROPERTY(BlueprintReadOnly, Category = "Settings button")
		TMap<FString, FString> ConsoleCommandsMap;

	// Called by OnSelectionChanged event
	UFUNCTION(BlueprintCallable, Category = "Settings button")
		void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
