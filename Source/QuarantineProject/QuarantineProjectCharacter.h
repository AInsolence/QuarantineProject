// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuarantineProjectCharacter.generated.h"

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
	/** Sniper aiming camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SniperAimingCamera;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Weapon component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponInHands;
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARifleProjectile_01> ProjectileClass;
	/** Projectiles shell class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARifleProjectile_01> ShellClass;
	/** Muzzle fire effect */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UParticleSystem* MuzzleFireEffect = nullptr;

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

	/** Aiming to target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimingSettings")
	bool bIsAiming = false;
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimationAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimationHip;

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

private:
	class AQP_HUD* GetPlayerHUD() const;
};

