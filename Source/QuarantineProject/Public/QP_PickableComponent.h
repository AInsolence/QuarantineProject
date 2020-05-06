// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QP_PickableComponent.generated.h"

/*
	This class is a component to make any actors pickable by the player in the game.
	Just add this component to the actor and describe to OnPickUp delegate.
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARANTINEPROJECT_API UQP_PickableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQP_PickableComponent();
	// calls when owner actor is picked up by player
	UFUNCTION()
	void PickUp();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	EPickableItemType ItemType = EPickableItemType::Rifle;
};
