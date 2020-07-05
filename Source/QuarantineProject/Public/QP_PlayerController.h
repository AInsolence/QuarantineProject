// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QUARANTINEPROJECT_API AQP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Called for actor in a start of the game */
	virtual void BeginPlay() override;
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Category = Setup)
	ACharacter* GetControlledCharacter() const;
	virtual void SetupInputComponent() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Camera control input */
	void AddControllerYawInput(float Value);
	void AddControllerPitchInput(float Value);

	/** This overrides the Crouch() in character */
	void Jump();
	void StopJumping();

	/** This overrides the Crouch() in character */
	void Crouch();
	void UnCrouch();

	/** This overrides the Crouch() in character */
	void SprintStart();
	void SprintEnd();

	/** Aiming */
	void AimToTarget();
	
	/** Fire */
	void OnFire();
	void OnStopFiring();
	void OnReloading();

	/** Items interaction */
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void HideWeapon();
	UFUNCTION()
	void ShowInventory();
	UFUNCTION()
	void Drop();
	UFUNCTION()
	void NextWeapon();
	UFUNCTION()
	void PreviousWeapon();

	/** Quit */
	void Exit();
	/** GameOver */
	UFUNCTION()
	void GameOver();
	
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
