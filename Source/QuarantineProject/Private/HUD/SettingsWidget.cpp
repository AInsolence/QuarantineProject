// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SettingsWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HUD/SettingsOptionWidget.h"
#include "HUD/SaveSettings.h"
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
		SaveSettingsInstance->SettingsValues = CurrentSettingsValues;
		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveSettingsInstance, "SettingsSlot", 0))
		{
			// Save succeeded.
			UE_LOG(LogTemp, Warning, TEXT("Save settings successfully"));
		}
	}
}

bool USettingsWidget::LoadSettingsValues()
{
	if (USaveSettings* LoadedSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		CurrentSettingsValues = LoadedSettings->SettingsValues;
		return true;
	}
	return false;
}
