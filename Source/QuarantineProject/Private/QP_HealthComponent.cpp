// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_HealthComponent.h"
#include "GameFramework/Actor.h"
#include "QuarantineProject/QuarantineProjectCharacter.h"
#include "Engine/World.h"
#include "QP_PlayerController.h"
#include "QP_HUD.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UQP_HealthComponent::UQP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = DefaultHealth;
	CurrentStamina = DefaultStamina;
}

void UQP_HealthComponent::ChangeCurrentStaminaTo(float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaCost, 0.0f, DefaultStamina);
}

// Called when the game starts
void UQP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UQP_HealthComponent::TakeDamage);
	}
}

void UQP_HealthComponent::TakeDamage(AActor* DamagedActor, 
										float Damage, 
										const UDamageType* DamageType, 
										AController* InstigatedBy, 
										AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, DefaultHealth);
}

