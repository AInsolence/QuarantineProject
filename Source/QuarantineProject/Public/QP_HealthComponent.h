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
	FORCEINLINE float GetDefaultHealth() const { return DefaultHealth; };
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; };
	FORCEINLINE float GetDefaultStamina() const { return DefaultStamina; };
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; };
	UFUNCTION()
	void ChangeCurrentStaminaTo(float StaminaCost);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultStamina = 100.f;
	UPROPERTY(BlueprintReadWrite, Category = "Health")
		float CurrentStamina;
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor,
						float Damage,
						const class UDamageType* DamageType,
						class AController* InstigatedBy,
						AActor* DamageCauser);
};
