// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QP_PickableComponent.h"
#include "Containers/List.h"
#include "Containers/StaticArray.h"
#include "QuarantineProject/Public/HUD/QP_InventorySlotWidget.h"
#include "QP_InventorySystemComponent.generated.h"

/*
	This class contains inventory and equip containers and logic to pick up, 
	store and manage pickable items by character.
*/

DECLARE_DYNAMIC_DELEGATE_OneParam(FItemCanBePickedUp, bool, bIsPickupTipNeedToBeShown);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARANTINEPROJECT_API UQP_InventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQP_InventorySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, Category = "Inventory")
	TDoubleLinkedList<UQP_InventorySlotWidget*> EquipedItemsContainer;
	TStaticArray<TStaticArray<bool, 12>, 2> EquipedItemsHUDRepresentation;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<UQP_InventorySlotWidget*> InventoryContainer;
	TStaticArray<TStaticArray<bool, 10>, 10> InventoryHUDRepresentation;

	// Add item to given inventory container
	template<int32 col, int32 row>
	bool AddItemToInventoryContainer(TStaticArray<TStaticArray<bool, col>, row> &TargetContainer, UQP_InventorySlotWidget* ItemWidget);
	// Returns free slot coordinates in given two dimensional container for given item size or FIntPoint(-1, -1)
	template<int32 col, int32 row>
	FIntPoint FindFreeSlotForItem(TStaticArray<TStaticArray<bool, col>, row> TargetContainer, FIntPoint ItemSize);
	// Insert item in given container
	template<int32 col, int32 row>
	void InsertItemInContainer(TStaticArray<TStaticArray<bool, col>, row> &TargetContainer,
																		FIntPoint ItemSize,
																		FIntPoint StartSlot);

	UFUNCTION()
	bool AddItemToInventory(UQP_InventorySlotWidget* ItemWidget);
	UFUNCTION()
	bool EquipItem(UQP_InventorySlotWidget* ItemWidget);
	UFUNCTION()
	bool ThrowItemFromInventory();

	APawn* Owner = nullptr;
	UWorld* World = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, 
								ELevelTick TickType, 
								FActorComponentTickFunction* ThisTickFunction) override;
	// event is called when item was picked up
	FItemCanBePickedUp OnItemCanBePickedUp;
	
	///*** API ***///
	UFUNCTION()
	// Get item from raycast. Return AActor* only if item is pickable, or nullptr.
	AActor* RaycastToFindPickableItem();
	UFUNCTION()
	void PickUpItem();
	UFUNCTION()
	void DropItem();
	UFUNCTION()
	FInventoryItemInfo NextWeapon(AActor* WeaponInHand);
	UFUNCTION()
	FInventoryItemInfo PreviousWeapon(AActor* WeaponInHand);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<EPickableItemType> AmmunitionTypeArray;
};

template<int32 col, int32 row>
bool UQP_InventorySystemComponent::AddItemToInventoryContainer(TStaticArray<TStaticArray<bool, col>, row> &TargetContainer,
																					UQP_InventorySlotWidget* ItemWidget)
{
	// check if item's class is valid
	if (ItemWidget->InventoryItemInfo.ItemClassPtr)
	{
		// check if place in container exist
		auto FreeSlot = FindFreeSlotForItem<col, row>(TargetContainer, ItemWidget->InventoryItemInfo.SizeInInventory);
		UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
		if (FreeSlot.X != -1)
		{// add item in container started from the found slot
			UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
			InsertItemInContainer<col, row>(TargetContainer, ItemWidget->InventoryItemInfo.SizeInInventory, FreeSlot);
			ItemWidget->InventoryItemInfo.PositionInInventory = FreeSlot;
			// add to inventory widget
			if (Owner)
			{
				auto Controller = Owner->GetController();
				if (Controller)
				{
					auto HUD = Cast<APlayerController>(Controller)->GetHUD();
					if (HUD)
					{
						if (ItemWidget)
						{// add item widget to HUD, in appropriate grid
							if (row == 2)
							{
								UE_LOG(LogTemp, Warning, TEXT("Add to equipment"));
								UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
								Cast<AQP_HUD>(HUD)->AddSlotToWeaponGrid(ItemWidget, FreeSlot);
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Add to inventory"));
								UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
								Cast<AQP_HUD>(HUD)->AddSlotToBackPackGrid(ItemWidget, FreeSlot);
							}
						}
					}
				}
			}
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Not enough place in given container");
			return false;
		}
	}
	return false;
}

template<int32 col, int32 row>
FIntPoint UQP_InventorySystemComponent::FindFreeSlotForItem(TStaticArray<TStaticArray<bool, col>, row> TargetContainer, FIntPoint ItemSize)
{
	// determine the possible search positions based on the size of the item
	int32 SearchRowSize = row - ItemSize.Y + 1;
	int32 SearchColumnSize = col - ItemSize.X + 1;

	///*** try to find position for top left item slot ***///
	// iterate by rows in grid
	for (int32 InternalRow = 0; InternalRow < SearchRowSize; ++InternalRow)
	{// iterate by columns in row
		for (int32 InternalColumn = 0; InternalColumn < SearchColumnSize; ++InternalColumn)
		{// check if slot is free
			if (TargetContainer[InternalRow][InternalColumn] == false)
			{/// *** try to find place for whole item ***///
				bool bIsNotPlaceForItem = false;
				for (int32 SizeRow = InternalRow; SizeRow < InternalRow + ItemSize.Y; ++SizeRow)
				{
					for (int32 SizeCol = InternalColumn; SizeCol < InternalColumn + ItemSize.X; ++SizeCol)
					{
						if (TargetContainer[InternalRow][InternalColumn] == true)
						{// one of slots is not available
							bIsNotPlaceForItem = true;
							break;
						} 
					}
					if (bIsNotPlaceForItem)
					{// one of slots is not available
						break;
					}
				}
				// after iterating by possible slots for that size -> all slots are empty
				if (!bIsNotPlaceForItem)
				{// then return first available slot
					return FIntPoint(InternalColumn, InternalRow);
				}
			}
		}
	}
	return FIntPoint(-1, -1);
}

template<int32 col, int32 row>
void UQP_InventorySystemComponent::InsertItemInContainer(TStaticArray<TStaticArray<bool, col>, row> &TargetContainer, FIntPoint ItemSize, FIntPoint StartSlot)
{
	for (int32 ItemCol = StartSlot.Y; ItemCol < StartSlot.Y + ItemSize.Y; ++ItemCol)
	{
		for (int32 ItemRow = StartSlot.X; ItemRow < StartSlot.X + ItemSize.X; ++ItemRow)
		{
			UE_LOG(LogTemp, Warning, TEXT("INSERT TO SLOT %d, %d"), ItemCol, ItemRow);
			TargetContainer[ItemCol][ItemRow] = true;
		}
	}
}
