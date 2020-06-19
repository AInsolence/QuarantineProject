// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/Inventory/QP_InventorySystemComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "QuarantineProject/Public/HUD/QP_HUD.h"
#include "QuarantineProject/Public/HUD/QP_InventoryUniformGridPanel.h"
#include "QuarantineProject/Public/HUD/QP_InventoryWidget.h"

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
	// bind inventory weapon grid change delegate
	if (Owner)
	{
		auto Controller = Owner->GetController();
		if (Controller)
		{
			auto HUD = Cast<APlayerController>(Controller)->GetHUD();
			if (HUD)
			{
				 auto WeaponGridPanel = Cast<AQP_HUD>(HUD)->InventoryWidget->WeaponGridPanel;
				 if (WeaponGridPanel)
				 {
					WeaponGridPanel->OnInventoryGridChanged.AddDynamic(this, &UQP_InventorySystemComponent::UpdateEquipedItems);
				 }
			}
		}
	}	
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
			if (HittedItemPickableComp->InventoryItemWidget)
			{
				if (AddItemToInventory(HittedItemPickableComp->InventoryItemWidget))
				{
					HittedItemPickableComp->PickUp();
					return;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Not enough place Anywhere");
				}
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
	//ThrowItemFromInventory();
}


bool UQP_InventorySystemComponent::AddItemToInventory(UQP_InventorySlotWidget* ItemWidget)
{
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
					if (AmmunitionTypeArray.Contains(ItemWidget->InventoryItemInfo.ItemType))
					{// if weapon -> add to weapon grid
						UE_LOG(LogTemp, Warning, TEXT("Add to equipment"));
						if (Cast<AQP_HUD>(HUD)->AddSlotToWeaponGrid(ItemWidget))
						{
							UpdateEquipedItems();
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Add to inventory"));
						return Cast<AQP_HUD>(HUD)->AddSlotToBackPackGrid(ItemWidget);
					}
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

void UQP_InventorySystemComponent::UpdateEquipedItems()
{
	if (Owner)
	{
		auto Controller = Owner->GetController();
		if (Controller)
		{
			auto HUD = Cast<APlayerController>(Controller)->GetHUD();
			if (HUD)
			{
				auto WeaponItems = Cast<AQP_HUD>(HUD)->GetWeaponGridItems();
				EquipedItemsContainer.Empty();
				for (auto Weapon : WeaponItems)
				{
					EquipedItemsContainer.AddTail(Weapon);
					UE_LOG(LogTemp, Warning, TEXT("Item in GRID: %s"), *Weapon->GetClass()->GetName());
				}
			}
		}
	}
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
			EquipedItemsContainer.AddTail(PickableComp->InventoryItemWidget);
		}
	}
	// get new weapon from equipment
	if (EquipedItemsContainer.GetHead())
	{
		if (EquipedItemsContainer.GetHead()->GetValue())
		{
			auto Weapon = EquipedItemsContainer.GetHead()->GetValue();
			EquipedItemsContainer.RemoveNode(EquipedItemsContainer.GetHead());
			//UE_LOG(LogTemp, Warning, TEXT("TRY TO EQUIP: %s"), Weapon->InventoryItemInfo.ItemClassPtr)
			return Weapon->InventoryItemInfo;
		}
		return FInventoryItemInfo();
	}
	return FInventoryItemInfo();
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
			EquipedItemsContainer.AddHead(PickableComp->InventoryItemWidget);
		}
	}
	// get new weapon from equipment
	if (EquipedItemsContainer.GetHead())
	{
		if (EquipedItemsContainer.GetHead()->GetValue())
		{
			auto Weapon = EquipedItemsContainer.GetTail()->GetValue();
			EquipedItemsContainer.RemoveNode(EquipedItemsContainer.GetTail());
			return Weapon->InventoryItemInfo;
		}
		return FInventoryItemInfo();
	}
	return FInventoryItemInfo();
}
