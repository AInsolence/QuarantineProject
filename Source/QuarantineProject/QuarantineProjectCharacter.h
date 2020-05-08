// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuarantineProject/Public/Inventory/QP_InventoryItem.h"
#include "QuarantineProjectCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverByPlayersDeath);

UCLASS(config=Game)
class AQuarantineProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQuarantineProjectCharacter();
protected:
	/** Called for actor in a start of the game */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	/** Aiming camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* AimingCamera;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UQP_HealthComponent* HealthComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	// Sprint variables
	bool bIsSprinting = false;
	float BaseWalkingSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float BaseSprintMultiplier = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float MaxSprintMultiplier = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float TimeToMaxSprintSpeed = 2.0f;


	class AQP_HUD* GetPlayerHUD() const;
	void ChangeWeapon(FInventoryItemInfo WeaponInfo);

public:
	/** Inventory system component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UQP_InventorySystemComponent* InventoryComponent;

	/** Weapon component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AQP_WeaponBase> WeaponInHandsClass;
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AQP_WeaponBase* WeaponInHands;

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
	UFUNCTION()
	void ShowFireAnimation();

	/** Inventory component */
	UFUNCTION()
	void PickUpItem();
	UFUNCTION()
	void DropItem();
	UFUNCTION()
	void NextWeapon();
	UFUNCTION()
	void PreviousWeapon();


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
	void OnStopFiring();
	void OnReloading();
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
	FGameOverByPlayersDeath OnGameOverByPlayersDeath;
};

