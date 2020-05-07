// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	UClass* ItemClassPtr = nullptr;
	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	EPickableItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	FIntPoint SizeInInventory = FIntPoint(1, 1);

	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	bool bIsCountable = false; // true for items which could be added in one pack
	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	int32 CurrentPackSize = 0; // set if item is countable
	UPROPERTY(EditAnywhere, Category = "InventoryItem")
	int32 MaxPackSize = 0; // set if item is countable
};
