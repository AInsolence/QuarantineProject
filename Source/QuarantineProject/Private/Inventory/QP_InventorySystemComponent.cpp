// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarantineProject/Public/Inventory/QP_InventorySystemComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

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
		{
			AddItemToInventory(HittedItemPickableComp->InventoryItemInfo);
			HittedItemPickableComp->PickUp();
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
	{
		if (AmmunitionTypeArray.Contains(ItemInfo.ItemType))
		{
			GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, "I find weapon and equip");
			EquipItem(ItemInfo);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, "I find smth and add to inventory");
			InventoryContainer.Add(ItemInfo);
		}
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I cannot add it to inventory");
		return false;
	}
}

bool UQP_InventorySystemComponent::EquipItem(FInventoryItemInfo ItemInfo)
{
	// check if item's class is valid
	if (ItemInfo.ItemClassPtr)
	{
		EquipedItemsContainer.Add(ItemInfo);
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

EPickableItemType UQP_InventorySystemComponent::NextWeapon()
{
	// check if ammunition array is not empty
	if (AmmunitionTypeArray.Num() > 0)
	{
		// check if current item is not the last in array
		if (AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) !=
			AmmunitionTypeArray.Num() - 1)
		{// set current weapon as next weapon in array
			CurrentWeapon = AmmunitionTypeArray[
				AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) + 1];
		}
		else
		{// set current weapon as first weapon in array
			CurrentWeapon = AmmunitionTypeArray[0];
		}
	}
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "NextWeapon");
	return CurrentWeapon;
}

EPickableItemType UQP_InventorySystemComponent::PreviousWeapon()
{
	// check if ammunition array is not empty
	if (AmmunitionTypeArray.Num() > 0)
	{
		// check if current item is not the last in array
		if (AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) > 0)
		{// set current weapon as next weapon in array
			CurrentWeapon = AmmunitionTypeArray[
				AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) - 1];
		}
		else
		{// set current weapon as first weapon in array
			CurrentWeapon = AmmunitionTypeArray.Last();
		}
	}
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "PreviousWeapon");
	return CurrentWeapon;
}
