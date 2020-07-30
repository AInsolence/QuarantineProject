// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Map.h"
#include "SettingsWidget.generated.h"

/**
 * 
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

    UPROPERTY(VisibleAnywhere, Category = "SaveSettings")
    TMap<FString, FString> CurrentSettingsValues;
    UFUNCTION(BlueprintCallable)
    void SaveCurrentSettingsValues();
    UFUNCTION(BlueprintCallable)
    void LoadCurrentSettingsValues();

    UFUNCTION(BlueprintCallable)
    void ApplyNewSettings();
    UFUNCTION(BlueprintCallable)
    void SaveSettingsValues();
    UFUNCTION(BlueprintCallable)
    bool LoadSettingsValues();
};
