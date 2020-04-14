// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QP_HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARANTINEPROJECT_API UQP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQP_HealthComponent();
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentStamina;
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor,
						float Damage,
						const class UDamageType* DamageType,
						class AController* InstigatedBy,
						AActor* DamageCauser);
};
