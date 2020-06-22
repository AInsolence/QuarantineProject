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
	TArray<UQP_InventorySlotWidget*> EquipedItemsContainer;

	UFUNCTION()
	bool AddItemToInventory(UQP_InventorySlotWidget* ItemWidget);
	UFUNCTION()
	void UpdateEquipedItems();

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
	UQP_InventorySlotWidget* NextWeapon();
	UFUNCTION()
	UQP_InventorySlotWidget* PreviousWeapon();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<EPickableItemType> AmmunitionTypeArray;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	UQP_InventorySlotWidget* ActiveWeapon;
};
