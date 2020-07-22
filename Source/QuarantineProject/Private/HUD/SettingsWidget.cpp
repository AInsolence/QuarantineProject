// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/HUD/SettingsWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "QuarantineProject/Public/HUD/SettingsOptionWidget.h"
#include "QuarantineProject/Public/HUD/SaveSettings.h"
#include "Engine.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitOptions();
	LoadCurrentSettingsValues();
}

void USettingsWidget::InitOptions()
{// check is the vertical layout for buttons exists
	if (VButtonsContainer)
	{// check is a settings option class exists
		if (SettingsOption)
		{
			if (SettingsButtonMap.Num() > 0)
			{// create all options was set in BP
				for (auto OptionInfo : SettingsButtonMap)
				{
					auto Option = CreateWidget<USettingsOptionWidget>(GetWorld(), SettingsOption);
					Option->SetName(OptionInfo.Key);
					// check are values for the option exist
					Option->ConsoleCommandPrefix = OptionInfo.Value.ConsoleCommandPrefix;
					if (OptionInfo.Value.ConsoleCommandValues.Num() > 0)
					{
						Option->SetValues(OptionInfo.Value.ConsoleCommandValues);
					}
					//// set default values from config file
					//auto CVar = IConsoleManager::Get().
					//	FindConsoleVariable(*OptionInfo.Value.ConsoleCommandPrefix);
					//UE_LOG(LogTemp, Warning, TEXT("!Before CVar, console command %s"), *OptionInfo.Value.ConsoleCommandPrefix)
					//if (CVar)
					//{
					//	UE_LOG(LogTemp, Warning, TEXT("!In CVar"))
					//	FString Value = "";
					//	Value = CVar->GetString();
					//	if (!Value.IsEmpty())
					//	{
					//		UE_LOG(LogTemp, Warning, TEXT("!In CVar String, %s"), *Value)
					//		auto OptionValueNameInComboBox = Option->ConsoleCommandsMap.FindKey(Value);
					//		if (OptionValueNameInComboBox)
					//		{
					//			Option->SetValue(*OptionValueNameInComboBox);
					//		}
					//	}
					//}
					VButtonsContainer->AddChildToVerticalBox(Option);
				}
			}
		}
	}
}

void USettingsWidget::ApplyNewSettings()
{
	SaveCurrentSettingsValues();
	GEngine->GetGameUserSettings()->SaveSettings();
}

void USettingsWidget::SaveCurrentSettingsValues()
{
	auto SettingsOptions = VButtonsContainer->GetAllChildren();

	for (auto option : SettingsOptions)
	{
		auto OptionWidget = Cast<USettingsOptionWidget>(option);
		FString OptionName = OptionWidget->ConsoleCommandPrefix;
		FString CurrentOptionValue = OptionWidget->OptionValues->GetSelectedOption();
		CurrentSettingsValues.Add(OptionName, CurrentOptionValue);
	}
	SaveSettingsValues();
}

void USettingsWidget::LoadCurrentSettingsValues()
{
	// load values from slot
	LoadSettingsValues();
	// set loaded settings values for each option
	auto SettingsOptions = VButtonsContainer->GetAllChildren();
	for (auto option : SettingsOptions)
	{
		auto OptionWidget = Cast<USettingsOptionWidget>(option);
		FString* CurrentOptionValue = CurrentSettingsValues.Find(OptionWidget->ConsoleCommandPrefix);
		if (CurrentOptionValue)
		{
			OptionWidget->SetValue(*CurrentOptionValue);
		}
	}
}


void USettingsWidget::SaveSettingsValues()
{
	if (USaveSettings* SaveSettingsInstance = Cast<USaveSettings>(UGameplayStatics::CreateSaveGameObject(USaveSettings::StaticClass())))
	{
		// Set data on the savegame object.
		SaveSettingsInstance->VideoSettingsValues = CurrentSettingsValues;
		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveSettingsInstance, "VideoSettingsSlot", 0))
		{
			// Save succeeded.
			UE_LOG(LogTemp, Warning, TEXT("Save settings successfully"));
		}
	}
}

void USettingsWidget::LoadSettingsValues()
{
	if (USaveSettings* LoadedVideoSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot("VideoSettingsSlot", 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		UE_LOG(LogTemp, Warning, TEXT("Load settings successfully"));
		CurrentSettingsValues = LoadedVideoSettings->VideoSettingsValues;
	}
}
