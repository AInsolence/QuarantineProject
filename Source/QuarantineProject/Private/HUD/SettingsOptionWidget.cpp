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
}

void USettingsOptionWidget::SetName(const FString Name)
{
	OptionName->SetText(FText::FromString(Name));
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
		OptionValues->AddOption(option.Key);
	}
}

void USettingsOptionWidget::SetValue(const FString Value)
{
	OptionValues->SetSelectedOption(Value);	
	// CONSOLE COMMAND VERSION
	// Create and execute console command to set option
	auto ConsoleCommandValue = ConsoleCommandsMap[Value];
	FString GeneratedConsoleCommand = ConsoleCommandPrefix + " " + ConsoleCommandValue;
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(*GeneratedConsoleCommand);
	//// Find appropriate key in config file to save
	//auto CVar = IConsoleManager::Get().FindConsoleVariable(*ConsoleCommandPrefix);
	//UE_LOG(LogTemp, Warning, TEXT("Before CVar, console command %s"), *ConsoleCommandPrefix)
	//if (CVar && !ConsoleCommandValue.IsEmpty())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("In CVar String, %s"), *Value)
	//	CVar->Set(*ConsoleCommandValue, ECVF_SetByConsole);
	//}
}

void USettingsOptionWidget::SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SetValue(SelectedItem);
}
