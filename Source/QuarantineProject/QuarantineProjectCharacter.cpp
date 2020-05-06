// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuarantineProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "QP_HUD.h"
#include "QP_HealthComponent.h"
#include "QuarantineProject/Public/QP_WeaponBase.h"
#include "QP_InventorySystemComponent.h"

//////////////////////////////////////////////////////////////////////////
// AQuarantineProjectCharacter

AQuarantineProjectCharacter::AQuarantineProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 340.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeLocation(FVector(0, 50.f, 90.f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetFieldOfView(110.f);

	// Create a follow camera
	AimingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimingCamera"));
	AimingCamera->SetupAttachment(GetCapsuleComponent()); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	AimingCamera->SetRelativeLocation(FVector(-50.f, 50.f, 90.f));
	AimingCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	AimingCamera->SetFieldOfView(80.f);

	// Create health component
	HealthComponent = CreateDefaultSubobject<UQP_HealthComponent>(TEXT("HealthComponent"));
	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UQP_InventorySystemComponent>(TEXT("InventoryComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////

void AQuarantineProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true; // allow to move camera up'n'down TRUE
	bUseControllerRotationRoll = false;

	// set base speed sprint variables
	BaseWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// On take damage subscribing
	OnTakeAnyDamage.AddDynamic(this, &AQuarantineProjectCharacter::OnTakeDamage);

	// Init weapon
	if (WeaponInHandsClass)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			WeaponInHands = World->SpawnActor<AQP_WeaponBase>(WeaponInHandsClass);
		}
	}

	if (WeaponInHands)
	{
		// Create weapon in character hands
		FVector InHandLocation = GetMesh()->GetSocketLocation(FName("RightHandWeaponSocket"));
		WeaponInHands->AttachToComponent(GetMesh(),
										FAttachmentTransformRules::SnapToTargetNotIncludingScale,
										FName("RightHandWeaponSocket"));
		// Describe to reloading event
		WeaponInHands->OnReloading.AddDynamic(this, &AQuarantineProjectCharacter::ShowReloadAnimation);
		// Describe to fire event
		WeaponInHands->OnFireEvent.AddDynamic(this, &AQuarantineProjectCharacter::ShowFireAnimation);

	}

}

void AQuarantineProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/**  Sprint logic */
	float RampThisFrame = (DeltaTime / TimeToMaxSprintSpeed) * MaxSprintMultiplier;
	if (bIsSprinting && HealthComponent->GetCurrentStamina() > 0.5f)
	{
		BaseSprintMultiplier += RampThisFrame;
		// change stamina bar percentage
		if (HealthComponent)
		{
			HealthComponent->ChangeCurrentStaminaTo(-0.5f);
			if (GetPlayerHUD() != nullptr)
			{
				GetPlayerHUD()->UpdateStaminaState(HealthComponent->GetCurrentStamina() / 100);
			}
		}

	}
	else
	{
		BaseSprintMultiplier -= RampThisFrame;
		// change stamina bar percentage
		if (HealthComponent)
		{
			HealthComponent->ChangeCurrentStaminaTo(0.1f);
			if (GetPlayerHUD() != nullptr)
			{
				GetPlayerHUD()->UpdateStaminaState(HealthComponent->GetCurrentStamina() / 100);
			}
		}
	}
	BaseSprintMultiplier = FMath::Clamp(BaseSprintMultiplier, 1.0f, MaxSprintMultiplier);
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkingSpeed * BaseSprintMultiplier;
	/**  Sprint logic end */
}

void AQuarantineProjectCharacter::ShowReloadAnimation()
{
	if (ReloadIronsightAnimation && ReloadHitAnimation)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			if (bIsAiming)
			{
				AnimInstance->Montage_Play(ReloadIronsightAnimation, 1.0f);
			}
			else
			{
				AnimInstance->Montage_Play(ReloadHitAnimation, 1.f);
			}
		}
	}
}

void AQuarantineProjectCharacter::ShowFireAnimation()
{
	// play a firing animation if specified
	if (FireAnimationHip && FireAnimationAiming)
	{
		if (!WeaponInHands->IsWeaponCanShoot()) return;
		// Get the animation object for the mesh
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			if (bIsAiming)
			{
				AnimInstance->Montage_Play(FireAnimationAiming, 1.f);
			}
			else
			{
				AnimInstance->Montage_Play(FireAnimationHip, 1.f);
			}
		}
	}
}

//*****            INPUT LOGIC                *****//

void AQuarantineProjectCharacter::PickUpItem()
{
	if (InventoryComponent)
	{
		auto PickableItem = InventoryComponent->RaycastToFindPickableItem();
		InventoryComponent->AddItemToInventory(PickableItem);
	}
}

void AQuarantineProjectCharacter::DropItem()
{
	if (InventoryComponent)
	{
		InventoryComponent->ThrowItemFromInventory();
	}
}

void AQuarantineProjectCharacter::NextWeapon()
{
	if (InventoryComponent)
	{
		InventoryComponent->NextWeapon();
	}
}

void AQuarantineProjectCharacter::PreviousWeapon()
{
	if (InventoryComponent)
	{
		InventoryComponent->PreviousWeapon();
	}
}

void AQuarantineProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AQuarantineProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AQuarantineProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

AQP_HUD* AQuarantineProjectCharacter::GetPlayerHUD() const
{// helper to get player HUD from controller
	if (GetWorld()->GetFirstPlayerController())
	{
		if (Cast<APlayerController>(GetController()))
		{
			auto HUDPtr = Cast<AQP_HUD>(Cast<APlayerController>(GetController())->GetHUD());
			if (HUDPtr)
			{
				return HUDPtr;
			}
			return nullptr;
		}
		return nullptr;
	}
	return nullptr;
}

void AQuarantineProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQuarantineProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AQuarantineProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AQuarantineProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AQuarantineProjectCharacter::Jump()
{
	Super::Jump();
}

void AQuarantineProjectCharacter::StopJumping()
{
	Super::StopJumping();
}

void AQuarantineProjectCharacter::AddControllerYawInput(float Value)
{
	Super::AddControllerYawInput(Value);
}

void AQuarantineProjectCharacter::AddControllerPitchInput(float Value)
{
	Super::AddControllerPitchInput(Value);
}

void AQuarantineProjectCharacter::Crouch()
{
	Super::Crouch();
}

void AQuarantineProjectCharacter::UnCrouch()
{
	Super::UnCrouch();
}

void AQuarantineProjectCharacter::SprintStart()
{
	bIsSprinting = true;
}

void AQuarantineProjectCharacter::SprintEnd()
{
	bIsSprinting = false;
}

void AQuarantineProjectCharacter::AimToTarget()
{// Switch aiming status
	bIsAiming = !bIsAiming;
	// Switch camera
	AimingCamera->SetActive(bIsAiming); //TODO change to real swith depends on the weapon type
	FollowCamera->SetActive(!bIsAiming);
	// Show crosshair in aiming mode
	GetPlayerHUD()->SetCrosshairVisibility(bIsAiming);
}

void AQuarantineProjectCharacter::OnFire()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (WeaponInHands)
		{
			FRotator MuzzleRotation;
			if (bIsAiming && !bIsSprinting)
			{
				MuzzleRotation = AimingCamera->GetComponentRotation();
				WeaponInHands->Fire(MuzzleRotation);
			}
			else
			{
				MuzzleRotation = WeaponInHands->GetMuzzleRotation();
				WeaponInHands->Fire(MuzzleRotation);
			}
		}
	}
}

void AQuarantineProjectCharacter::OnStopFiring()
{
	if (WeaponInHands)
	{
		WeaponInHands->StopFiring();
	}
}

void AQuarantineProjectCharacter::OnReloading()
{
	if (WeaponInHands)
	{
		WeaponInHands->Reloading();
	}
}

//*****                                        *****//

void AQuarantineProjectCharacter::OnTakeDamage(AActor* DamagedActor, 
												float Damage, 
												const UDamageType* DamageType,
												AController* InstigatedBy,
												AActor* DamageCauser)
{
	if (HealthComponent) 
	{

		// try and play injured animation if specified
		if (InjuredAnimation)
		{
			// Get the animation object for the mesh
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(InjuredAnimation, 2.0f);
			}
		}
		//Update HUD health status
		if (GetPlayerHUD())
		{
			GetPlayerHUD()->UpdateHealthState(HealthComponent->GetCurrentHealth() / 100);
		}
		//*** DEATH ***//
		if (HealthComponent->GetCurrentHealth() <= 0)
		{
			OnGameOverByPlayersDeath.Broadcast();
			// Disable collision capsule if the character is dead
			auto Capsule = DamagedActor->FindComponentByClass<UCapsuleComponent>();
			if (Capsule)
			{
				Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			// Disable controller
			AController* CurrentController = GetController();
			if (CurrentController) {
				// stop movement so the death animation plays immediately
				CurrentController->StopMovement();
				/* AI logic option */
				// unpossess to stop AI
				//CurrentController->UnPossess();
				// destroy the controller, since it's not part of the enemy anymore
				//CurrentController->Destroy();
			}
		}
		// *** DEATH END *** //
	}
}
