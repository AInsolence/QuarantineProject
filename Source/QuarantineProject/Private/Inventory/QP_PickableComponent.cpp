// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/Inventory/QP_PickableComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"

// Sets default values for this component's properties
UQP_PickableComponent::UQP_PickableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UQP_PickableComponent::BeginPlay()
{
	if (GetOwner())
	{
		if (InventoryItemWidget)
		{
			InventoryItemWidget->InventoryItemInfo.ItemClassPtr = GetOwner()->GetClass();
		}
	}
}


void UQP_PickableComponent::PickUp()
{
	UE_LOG(LogTemp, Warning, TEXT("PickUp called"))
	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy called"))
		GetOwner()->Destroy();
	}
}
