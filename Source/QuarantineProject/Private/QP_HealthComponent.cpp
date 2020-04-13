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
	if (Damage <= 0.0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, DefaultHealth);

	// check if causer not a player itself and update HUD health status
	if (InstigatedBy != GetWorld()->GetFirstPlayerController())
	{
		auto PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				auto HUD = Cast<AQP_HUD>(PlayerController->GetHUD());
				if (HUD)
				{
					HUD->UpdateHealthState(CurrentHealth/DefaultHealth);
				}
			}
	}
	
	// Death
	if (CurrentHealth <= 0.0f)
	{// disable capsule collison after characters death
		auto Capsule = DamagedActor->FindComponentByClass<UCapsuleComponent>();
		if (Capsule)
		{
			Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth)
}

