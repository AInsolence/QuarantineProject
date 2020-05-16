// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/Inventory/QP_InventorySystemComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "QuarantineProject/Public/HUD/QP_HUD.h"

// Sets default values for this component's properties
UQP_InventorySystemComponent::UQP_InventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// create ammunition type list to use switching
	AmmunitionTypeArray.Add(EPickableItemType::ColdSteel);
	AmmunitionTypeArray.Add(EPickableItemType::Pistol);
	AmmunitionTypeArray.Add(EPickableItemType::Rifle);
	AmmunitionTypeArray.Add(EPickableItemType::LongWeapon);
	AmmunitionTypeArray.Add(EPickableItemType::Grenade);
	// fill HUD representation array with empty slots
	for (auto row : InventoryHUDRepresentation)
	{
		for (auto col : row)
		{
			col = false;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Inventory HUD grid is constructed"))
	for (auto row : EquipedItemsHUDRepresentation)
	{
		for (auto col : row)
		{
			col = false;
		}
	}	
	UE_LOG(LogTemp, Warning, TEXT("Equipment HUD grid is constructed"))
}


// Called when the game starts
void UQP_InventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// init
	World = GetWorld();
	if (GetOwner())
	{
		Owner = Cast<APawn>(GetOwner());
	}
	//// Show container
	//for (auto row : InventoryHUDRepresentation)
	//{
	//	FString str = "";
	//	for (auto col : row)
	//	{
	//		if (col) str += "1";
	//		else str += "0";
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *str)
	//}

	//auto FreeSlot = FindFreeSlotForItem<10, 10>(InventoryHUDRepresentation, FIntPoint(4, 2));
	//UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot.X, FreeSlot.Y);
	//InsertItemInContainer<10, 10>(InventoryHUDRepresentation, FIntPoint(4, 2), FreeSlot);

	//// Show container
	//for (auto row : InventoryHUDRepresentation)
	//{
	//	FString str = "";
	//	for (auto col : row)
	//	{
	//		if (col) str += "1";
	//		else str += "0";
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *str)
	//}
	//auto FreeSlot_1 = FindFreeSlotForItem<10, 10>(InventoryHUDRepresentation, FIntPoint(4, 2));
	//UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot_1.X, FreeSlot_1.Y);
	//InsertItemInContainer<10, 10>(InventoryHUDRepresentation, FIntPoint(4, 2), FreeSlot_1);
	//// Show container
	//for (auto row : InventoryHUDRepresentation)
	//{
	//	FString str = "";
	//	for (auto col : row)
	//	{
	//		if (col) str += "1";
	//		else str += "0";
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *str)
	//}
	//auto FreeSlot_2 = FindFreeSlotForItem<10, 10>(InventoryHUDRepresentation, FIntPoint(2, 2));
	//UE_LOG(LogTemp, Warning, TEXT("Free slot %d, %d"), FreeSlot_2.X, FreeSlot_2.Y);
	//InsertItemInContainer<10, 10>(InventoryHUDRepresentation, FIntPoint(2, 2), FreeSlot_2);
	//// Show container
	//for (auto row : InventoryHUDRepresentation)
	//{
	//	FString str = "";
	//	for (auto col : row)
	//	{
	//		if (col) str += "1";
	//		else str += "0";
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *str)
	//}
}

// Called every frame
void UQP_InventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// raycast to find pickable components on the map
	RaycastToFindPickableItem();
}

AActor* UQP_InventorySystemComponent::RaycastToFindPickableItem()
{
	if (World && Owner)
	{// get camera location and forward vector
		FVector RayStart;
		FRotator RayRotation;
		Owner->GetController()->GetPlayerViewPoint(RayStart, RayRotation);
		// set ray direction and distance
		FVector RayDirection = RayRotation.Vector();
		float RayDistance = 400.f;
		FVector RayEnd = RayStart + (RayDirection * RayDistance);
		// hit
		FHitResult HitResult;
		bool bHasHitSmth = GetWorld()->LineTraceSingleByChannel(HitResult,
													RayStart,
													RayEnd,
													ECollisionChannel::ECC_WorldDynamic);
		// if hit smth check if an item is pickable
		if (bHasHitSmth)
		{
			auto HittedActor = HitResult.GetActor();
			// check if actor is pickable
			auto ActorPickableComponent = HittedActor->FindComponentByClass<UQP_PickableComponent>();
			if (ActorPickableComponent)
			{// if it is pickable show the pick up option to player
				OnItemCanBePickedUp.ExecuteIfBound(true);
				return HittedActor;
			}
			else
			{
				OnItemCanBePickedUp.ExecuteIfBound(false);
				// item is not pickable
				return nullptr;
			}
		}
		OnItemCanBePickedUp.ExecuteIfBound(false);
		// nothing was hit
		return nullptr;
	}
	OnItemCanBePickedUp.ExecuteIfBound(false);
	// cannot find world or owner
	return nullptr;
}

void UQP_InventorySystemComponent::PickUpItem()
{
	// get hitted actor
	auto HittedActor = RaycastToFindPickableItem();
	if (HittedActor)
	{
		// try to get pickable component
		auto HittedItemPickableComp = HittedActor->FindComponentByClass<UQP_PickableComponent>();
		if (HittedItemPickableComp)
		{// try to add item in inventory
			if (AddItemToInventory(HittedItemPickableComp->InventoryItemInfo))
			{
				HittedItemPickableComp->PickUp();
				return;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Not enough place Anywhere");
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I cannot pick up it");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I cannot pick any");
	}
}

void UQP_InventorySystemComponent::DropItem()
{
	ThrowItemFromInventory();
}

bool UQP_InventorySystemComponent::AddItemToInventory(FInventoryItemInfo ItemInfo)
{
	// check if item's class is valid
	if (ItemInfo.ItemClassPtr)
	{// equip if it is a weapon
		if (AmmunitionTypeArray.Contains(ItemInfo.ItemType))
		{// try to equip
			if (EquipItem(ItemInfo))
			{
				GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, "I find weapon and equip");
				return true;
			}
			// try to add to inventory if is not enough place in equipment
			if (AddItemToInventoryContainer<10, 10>(InventoryHUDRepresentation, ItemInfo))
			{
				InventoryContainer.Add(ItemInfo);
				GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, "I find weapon and add to inventory");
				return true;
			}
			return false;
		}
		else
		{// just add to inventory if it is not a weapon
			if (AddItemToInventoryContainer<10, 10>(InventoryHUDRepresentation, ItemInfo))
			{
				InventoryContainer.Add(ItemInfo);
				GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, "I find smth and add to inventory");
				return true;
			}
			return false;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I cannot add it to inventory");
		return false;
	}
}

bool UQP_InventorySystemComponent::EquipItem(FInventoryItemInfo ItemInfo)
{
	if (AddItemToInventoryContainer<12, 2>(EquipedItemsHUDRepresentation,ItemInfo))
	{
		EquipedItemsContainer.AddTail(ItemInfo);
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I cannot equip it");
		return false;
	}
}

bool UQP_InventorySystemComponent::ThrowItemFromInventory()
{
	if (World)
	{
		// Set spawn parameters
		FVector ViewLocation;
		FRotator ViewRotation;
		Owner->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
		FVector SpawnLocation = ViewLocation + (ViewRotation.Vector() * 400);
		//Set spawn collision handling
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		if (InventoryContainer.Num() >= 1)
		{// try to throw(spawn) item in the world
			if (World->SpawnActor<AActor>(InventoryContainer.Pop().ItemClassPtr,
				SpawnLocation,
				FRotator(0, 0, 0),
				ActorSpawnParams))
			{
				return true;
			}
			return false;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Inventory empty");
			return false;
		}
	}
	return false;
}

FInventoryItemInfo UQP_InventorySystemComponent::NextWeapon(AActor* WeaponInHand)
{
	// check if any weapons equipped
	if (EquipedItemsContainer.Num() <= 0) return FInventoryItemInfo();
	// return first element and add WeaponInHand to equipment
	if (WeaponInHand)
	{
		auto PickableComp = WeaponInHand->FindComponentByClass<UQP_PickableComponent>();
		if (PickableComp)
		{// move weapon from hands to equipment
			EquipItem(PickableComp->InventoryItemInfo);
		}
	}
	// get new weapon from equipment
	auto Weapon = EquipedItemsContainer.GetHead()->GetValue();
	EquipedItemsContainer.RemoveNode(EquipedItemsContainer.GetHead());
	UE_LOG(LogTemp, Warning, TEXT("TRY TO EQUIP: %s"), Weapon.ItemClassPtr)
	return Weapon;
}

FInventoryItemInfo UQP_InventorySystemComponent::PreviousWeapon(AActor* WeaponInHand)
{
	// check if any weapons equipped
	if (EquipedItemsContainer.Num() <= 0) return FInventoryItemInfo();
	// return first element and add WeaponInHand to equipment
	if (WeaponInHand)
	{
		auto PickableComp = WeaponInHand->FindComponentByClass<UQP_PickableComponent>();
		if (PickableComp)
		{// move weapon from hands to equipment
			EquipedItemsContainer.AddHead(PickableComp->InventoryItemInfo);
		}
	}
	// get new weapon from equipment
	auto Weapon = EquipedItemsContainer.GetTail()->GetValue();
	EquipedItemsContainer.RemoveNode(EquipedItemsContainer.GetTail());
	UE_LOG(LogTemp, Warning, TEXT("TRY TO EQUIP: %s"), Weapon.ItemClassPtr)
	return Weapon;
}
