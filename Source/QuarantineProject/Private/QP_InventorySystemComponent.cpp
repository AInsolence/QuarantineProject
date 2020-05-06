// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_InventorySystemComponent.h"
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

	// create ammunition
	EquipedItemsContainer.Add(EPickableItemType::Helmet, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::Armor, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::ColdSteel, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::Pistol, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::Rifle, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::LongWeapon, nullptr);
	EquipedItemsContainer.Add(EPickableItemType::Grenade, nullptr);
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
		//DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, 0.2f, 0, 5.f);
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

void UQP_InventorySystemComponent::AddItemToInventory(AActor* HittedActor)
{
	if (!HittedActor) return;

	auto PickableItem = HittedActor->GetClass();
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Pickable item: " + PickableItem->GetName());
	// check if pickable item exist
	if (PickableItem)
	{
		auto PickupComponent = HittedActor->FindComponentByClass<UQP_PickableComponent>();
		InventoryContainer.Add(PickableItem);
		if (PickupComponent)
		{
			// try to equip item
			EquipItem(PickableItem, PickupComponent->ItemType);
			// execute delegate
			PickupComponent->PickUp();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I Cannot take it");
		return;
	}
}

bool UQP_InventorySystemComponent::EquipItem(UClass* PickedItem, EPickableItemType ItemType)
{
	// if item already equiped
	if (EquipedItemsContainer.FindRef(ItemType))
	{// move it to inventory
		auto Item = EquipedItemsContainer[ItemType]->GetClass();
		GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, "Substituted: ");
	}
	EquipedItemsContainer[ItemType] = PickedItem;
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, 
									 "Item equiped: " + PickedItem->GetName() + 
									 " Item type: " + 
									 UEnum::GetValueAsString <EPickableItemType>(ItemType));
	return true;
}

void UQP_InventorySystemComponent::ThrowItemFromInventory()
{
	if (World)
	{
		// set spawn parameters
		FVector ViewLocation;
		FRotator ViewRotation;
		Owner->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
		FVector SpawnLocation = ViewLocation + (ViewRotation.Vector() * 400);
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		if (InventoryContainer.Num() >= 1)
		{// try to throw(spawn) item in the world
			if (World->SpawnActor<AActor>(InventoryContainer.Pop(),
				SpawnLocation,
				FRotator(0, 0, 0),
				ActorSpawnParams))
			{// remove item reference from inventory after spawning
				//InventoryContainer.Pop();
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Inventory empty");
		}
	}
}

void UQP_InventorySystemComponent::NextWeapon()
{
	// check if ammunition array is not empty
	if (AmmunitionTypeArray.Num() > 0)
	{
		// check if current item is not the last in array
		if (AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) !=
			AmmunitionTypeArray.Num() - 1)
		{// set next type in array
			CurrentWeapon = AmmunitionTypeArray[
				AmmunitionTypeArray.IndexOfByKey(CurrentWeapon) + 1];
		}
		else
		{// set first type
			CurrentWeapon = AmmunitionTypeArray[0];
		}
	}
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "NextWeapon");
}

void UQP_InventorySystemComponent::PreviousWeapon()
{
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "PreviousWeapon");
}
