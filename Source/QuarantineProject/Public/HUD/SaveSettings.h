// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Containers/Map.h"
#include "SaveSettings.generated.h"

/**
	Class manages the settings save data
 */

UCLASS()
class QUARANTINEPROJECT_API USaveSettings : public USaveGame
{
	GENERATED_BODY()

public:
	USaveSettings();
	
	UPROPERTY(VisibleAnywhere, Category = "SaveSettings")
	uint32 UserIndex = 0;
	UPROPERTY(VisibleAnywhere, Category = "SaveSettings")
	FString SettingsSlot;
	UPROPERTY(VisibleAnywhere, Category = "SaveSettings")
	TMap<FString, FString> SettingsValues;
};
