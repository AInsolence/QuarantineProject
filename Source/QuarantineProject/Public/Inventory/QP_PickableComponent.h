// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarantineProject/Public/HUD/QP_InventorySlotWidget.h"
#include "QP_PickableComponent.generated.h"

/*
	This class is a component to make any actors will be pickable by the player in the game.
	Just add this component to the actor and describe to OnPickUp delegate.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARANTINEPROJECT_API UQP_PickableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:	
	// Sets default values for this component's properties
	UQP_PickableComponent();
	// calls when owner actor is picked up by player
	UFUNCTION()
	void PickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickableItemProperties")
	UQP_InventorySlotWidget* InventoryItemWidget;
};
