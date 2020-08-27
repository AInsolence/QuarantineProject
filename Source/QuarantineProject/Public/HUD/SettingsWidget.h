// Copyright © 2020 Insolence Assets, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Map.h"
#include "SettingsWidget.generated.h"

/**
    Class represents the main settings widget and store all values
    which related with options expandable in Blueprint child class.
 */

USTRUCT(BlueprintType)
struct FSystemSettingButton
{
public:
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings button")
        FString ConsoleCommandPrefix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings button")
        /* Key - a displaying option value name (eg low/medium/high),
           Value - a real value for console command */
        TMap<FString, FString> ConsoleCommandValues;
};

UCLASS()
class QUARANTINEPROJECT_API USettingsWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    // Constructor
    USettingsWidget(const FObjectInitializer& ObjectInitializer);
    // Native constructor
    virtual void NativeConstruct() override;
    // Calls to create all settings options
    void InitOptions();

    // Settings option widget class to set in BP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings button")
        TSubclassOf<class USettingsOptionWidget> SettingsOption;
    // Container to store buttons information for each settings button
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings button")
        TMap<FString, FSystemSettingButton> SettingsButtonMap;
    // UMG container for buttons
    UPROPERTY(BlueprintReadWrite, Category = "Settings button", meta = (BindWidget))
        class UScrollBox* ScrollBox;
    UPROPERTY(BlueprintReadWrite, Category = "Settings button", meta = (BindWidget))
        class UVerticalBox* VButtonsContainer;
    // Property and methods to work with current settings values stored in this class
    UPROPERTY(VisibleAnywhere, Category = "Save Settings")
        TMap<FString, FString> CurrentSettingsValues;
    UFUNCTION(BlueprintCallable, Category = "Save Settings")
        void SaveCurrentSettingsValues();
    UFUNCTION(BlueprintCallable, Category = "Save Settings")
        void LoadCurrentSettingsValues();

    // Methods to work with save/load settings 
    UFUNCTION(BlueprintCallable, Category = "Save Settings")
        void ApplyNewSettings();
    UFUNCTION(BlueprintCallable, Category = "Save Settings")
        void SaveSettingsToSlot();
    UFUNCTION(BlueprintCallable, Category = "Save Settings")
        bool LoadSettingsFromSlot();
};
