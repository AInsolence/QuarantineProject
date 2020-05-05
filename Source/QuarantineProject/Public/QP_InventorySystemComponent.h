// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
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
	TMap<int32, UClass*> InventoryContainer;
	APawn* Owner = nullptr;
	UWorld* World = nullptr;
	AActor* RaycastToFindPickableItem();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, 
								ELevelTick TickType, 
								FActorComponentTickFunction* ThisTickFunction) override;

	FItemCanBePickedUp OnItemCanBePickedUp;
	// get item from raycast and try to add to inventory
	void AddItemToInventory();
	void ThrowItemFromInventory();
};
