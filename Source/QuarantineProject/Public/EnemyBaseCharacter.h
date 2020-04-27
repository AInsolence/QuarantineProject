// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBaseCharacter.generated.h"

/**
	Class represents an enemy character which has an AIPerception component and
	is controlled by AI controller 
 */

UCLASS()
class QUARANTINEPROJECT_API AEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();
protected:
	/** Called for actor in a start of the game */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UQP_HealthComponent* HealthComponent;

	/** AI perception component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* PerceptionComponent;

	/** Weapon component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AQP_WeaponBase> WeaponInHandsClass;
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	AQP_WeaponBase* WeaponInHands;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.f;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.f;
	// Sprint variables
	bool bIsSprinting = false;
	float BaseWalkingSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float BaseSprintMultiplier = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float MaxSprintMultiplier = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float TimeToMaxSprintSpeed = 2.0f;

	/** Aiming to target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimingSettings")
	bool bIsAiming = false;

	/** AnimMontages to play */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimationAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimationHip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* ReloadIronsightAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* ReloadHitAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* InjuredAnimation;

	/** Play anim montages functions */
	UFUNCTION()
	void ShowReloadAnimation();

public:
	/** Resets HMD orientation in VR. */
	void OnResetVR();
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/** This overrides the Jump() in character */
	void Jump();
	void StopJumping();
	/** Camera control input override*/
	void AddControllerYawInput(float Value);
	void AddControllerPitchInput(float Value);
	/** This overrides the Crouch() in character */
	void Crouch();
	void UnCrouch();
	/** Sprinting */
	void SprintStart();
	void SprintEnd();
	/** Aiming */
	void AimToTarget();
	/** Firing */
	void OnFire();
	/** OnTakeAnyDamage event function */
	UFUNCTION()
		void OnTakeDamage(AActor* DamagedActor,
			float Damage,
			const class UDamageType* DamageType,
			class AController* InstigatedBy,
			AActor* DamageCauser);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
};
