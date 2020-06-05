// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "QP_InventoryItem.generated.h"

/**
	Class stores information about item inside inventory system 
 */

UENUM()
enum class EPickableItemType
{
	Ammo,
	Helmet,
	Armor,
	FirstAidKit,
	ColdSteel,
	Pistol,
	Rifle,
	LongWeapon,
	Grenade
};

USTRUCT(BlueprintType)
struct FInventoryItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	UClass* ItemClassPtr = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	EPickableItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	FIntPoint SizeInInventory = FIntPoint(1, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	FIntPoint PositionInInventory = FIntPoint(-1, -1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	bool bIsCountable = false; // true for items which could be added in one pack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	int32 CurrentPackSize = 0; // set if item is countable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItem")
	int32 MaxPackSize = 0; // set if item is countable
};
