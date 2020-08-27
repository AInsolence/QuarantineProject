// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SettingsOptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Engine.h"
#include "HAL/IConsoleManager.h"

USettingsOptionWidget::USettingsOptionWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void USettingsOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Bind combo box option changes function
	if (OptionValues)
	{
		OptionValues->OnSelectionChanged.AddDynamic(this,
			&USettingsOptionWidget::SelectionChanged);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Option values combo box does not exist. Please check BP_SettingsWidget -> Graph -> Details -> SettingsButton -> SettingsOption might be a widget contains 'OptionName' text area and 'OptionValues'combo box (BP_OptionWidget)."));
	}
}

void USettingsOptionWidget::SetName(const FString Name)
{
	if (OptionName)
	{
		OptionName->SetText(FText::FromString(Name));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Option name does not exist. Please check BP_SettingsWidget->Graph->Details->SettingsButton->SettingsOption might be a widget contains 'OptionName' text area and 'OptionValues'combo box(BP_OptionWidget)."));
	}
}

void USettingsOptionWidget::SetValues(const TMap<FString, FString> Values)
{
	if (Values.Num() < 1)
	{
		return;
	}
	// copy option values
	ConsoleCommandsMap = Values;
	// set options name as a combobox option
	for (auto option : Values)
	{
		if (OptionValues)
		{
			OptionValues->AddOption(option.Key);
		}
	}
}

void USettingsOptionWidget::SetValue(const FString Value)
{
	if (OptionValues)
	{
		OptionValues->SetSelectedOption(Value);
		// Create and execute console command to set option
		auto ConsoleCommandValue = ConsoleCommandsMap.Find(Value);
		if (ConsoleCommandValue)
		{
			FString GeneratedConsoleCommand = ConsoleCommandPrefix + " " + *ConsoleCommandValue;
			GetWorld()->GetFirstPlayerController()->ConsoleCommand(*GeneratedConsoleCommand);
		}
	}
}

void USettingsOptionWidget::SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SetValue(SelectedItem);
}
