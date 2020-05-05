// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_InventorySystemComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "QP_PickableComponent.h"

// Sets default values for this component's properties
UQP_InventorySystemComponent::UQP_InventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
		DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, 0.2f, 0, 5.f);
		// if hit smth check if an item is pickable
		if (bHasHitSmth)
		{
			auto HittedActor = HitResult.GetActor();
			// check if actor is pickable
			auto ActorPickableComponent = HittedActor->FindComponentByClass<UQP_PickableComponent>();
			if (ActorPickableComponent)
			{// if it is pickable show the pick up option to player
				OnItemCanBePickedUp.ExecuteIfBound();
				return HittedActor;
			}
			// item is not pickable
			return nullptr;
		}
		// nothing was hit
		return nullptr;
	}
	// cannot find world or owner
	return nullptr;
}

void UQP_InventorySystemComponent::AddItemToInventory()
{
	auto HittedActor = RaycastToFindPickableItem();
	if (!HittedActor) return;

	auto PickableItem = HittedActor->GetClass();
	GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Pickable item: " + PickableItem->GetName());
	// check if pickable item exist
	if (PickableItem)
	{
		InventoryContainer.Add(1, PickableItem);
		HittedActor->FindComponentByClass<UQP_PickableComponent>()->PickUp();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "I Cannot take it");
		return;
	}
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
		if (InventoryContainer.Contains(1))
		{// try to throw(spawn) item in the world
			if (World->SpawnActor<AActor>(InventoryContainer[1],
				SpawnLocation,
				FRotator(0, 0, 0),
				ActorSpawnParams))
			{// remove item reference from inventory after spawning
				InventoryContainer.Remove(1);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Red, "Inventory empty");
		}
	}
}

