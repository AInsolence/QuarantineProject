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
#include "HUD/QP_HUD.h"
#include "QP_HealthComponent.h"
#include "QP_WeaponBase.h"
#include "Inventory/QP_InventorySystemComponent.h"

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
	AimingCamera->SetupAttachment(GetCapsuleComponent());
	AimingCamera->SetRelativeLocation(FVector(-50.f, 50.f, 90.f));
	AimingCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	AimingCamera->SetFieldOfView(80.f);

	// Create health component
	HealthComponent = CreateDefaultSubobject<UQP_HealthComponent>(TEXT("HealthComponent"));
	// Create inventory component
	InventorySystemComponent = CreateDefaultSubobject<UQP_InventorySystemComponent>(TEXT("InventorySystemComponent"));
	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////

void AQuarantineProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*AimingCamera->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("headSocket"));*/
	//AimingCamera->SetupAttachment(GetCapsuleComponent());
	//AimingCamera->SetRelativeLocation(FVector(-50.f, 50.f, 90.f));
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true; // allow to move camera up and down TRUE
	bUseControllerRotationRoll = false;

	// set base speed sprint variables
	BaseWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// On take damage subscribing
	OnTakeAnyDamage.AddDynamic(this, &AQuarantineProjectCharacter::OnTakeDamage);

	// Init weapon
	if (WeaponInHandsClass)
	{
		if (GetWorld())
		{
			WeaponInHands = GetWorld()->SpawnActor<AQP_WeaponBase>(WeaponInHandsClass);
		}
	}
	InitWeaponSettings();
}

void AQuarantineProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/**  Sprint logic */
	float RampThisFrame = (DeltaTime / TimeToMaxSprintSpeed) * MaxSprintMultiplier;
	// check if sprint input pressed, is character has enough stamina and is he moves
	if (bIsSprinting && !this->GetVelocity().IsZero() &&
		HealthComponent->GetCurrentStamina() > 0.5f)	
	{
		BaseSprintMultiplier += RampThisFrame;
		// change stamina bar percentage
		if (HealthComponent)
		{
			HealthComponent->ChangeCurrentStaminaTo(-0.5f);
			if (GetPlayerHUD() != nullptr)
			{
				GetPlayerHUD()->UpdateStaminaState(HealthComponent->GetCurrentStamina() / 
														HealthComponent->GetDefaultStamina());
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
				GetPlayerHUD()->UpdateStaminaState(HealthComponent->GetCurrentStamina() / 
														HealthComponent->GetDefaultStamina());
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
				AnimInstance->Montage_Play(ReloadHitAnimation, 1.0f);
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

//*****            INPUT LOGIC                *****//

void AQuarantineProjectCharacter::PickUpItem()
{
	if (InventorySystemComponent)
	{
		InventorySystemComponent->PickUpItem();
	}
}

void AQuarantineProjectCharacter::DropItem()
{
	if (InventorySystemComponent)
	{
		InventorySystemComponent->DropItem();
	}
}

void AQuarantineProjectCharacter::NextWeapon()
{
	if (!IsWeaponCanBeChanged()) return;
	// set weapon to equip
	NextWeaponInfo = InventorySystemComponent->NextWeapon();
	// play equipping animation
	ShowEquipingAnimation();
}

void AQuarantineProjectCharacter::PreviousWeapon()
{
	if (!IsWeaponCanBeChanged()) return;
	// set weapon to equip
	NextWeaponInfo = InventorySystemComponent->PreviousWeapon();
	// play equipping animation
	ShowEquipingAnimation();
}

bool AQuarantineProjectCharacter::IsWeaponCanBeChanged() const
{
	if (!InventorySystemComponent)
	{
		return false;
	}
	if (bIsWeaponEquipping)
	{
		return false;
	}
	if (WeaponInHands && InventorySystemComponent->ActiveWeapon)
	{// check the inventory item and weapon in hand are the same
		auto InHandsClass = WeaponInHands->GetClass()->GetName();
		auto InventoryItemClass = InventorySystemComponent->ActiveWeapon->
			InventoryItemInfo.ItemClassPtr->GetName();
		if (bIsWeaponInHands && !InventorySystemComponent->CanWeaponBeChanged()
			&& InHandsClass == InventoryItemClass)
		{
			return false;
		}
	}
	return true;
}

void AQuarantineProjectCharacter::ShowEquipingAnimation()
{
	// try and play equip animation if specified
	if (EquipWeaponAnimation)
	{
		// Get the animation object for the mesh
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{// start equipping process
			bIsWeaponEquipping = true;
			AnimInstance->Montage_Play(EquipWeaponAnimation, 1.0f);
		}
	}
}

void AQuarantineProjectCharacter::ChangeWeapon()
{
	if (NextWeaponInfo == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No CLASS TO SPAWN"))
		return;
	}
	if (GetWorld())
	{
		if (NextWeaponInfo->InventoryItemInfo.ItemClassPtr)
		{
			if (WeaponInHands != nullptr)
			{
				WeaponInHands->Destroy();
			}// create new weapon in hands
			WeaponInHands = GetWorld()->SpawnActor<AQP_WeaponBase>(NextWeaponInfo->InventoryItemInfo.ItemClassPtr);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No CLASS TO SPAWN"))
		}
	}
	InitWeaponSettings();
}

void AQuarantineProjectCharacter::InitWeaponSettings()
{
	if (WeaponInHands != nullptr)
	{
		// Set weapon location as character hands
		FVector InHandLocation = GetMesh()->GetSocketLocation(FName("RightHandWeaponSocket"));
		WeaponInHands->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("RightHandWeaponSocket"));
		// Describe to reloading event
		WeaponInHands->OnReloading.AddDynamic(this, &AQuarantineProjectCharacter::ShowReloadAnimation);
		// Describe to fire event
		WeaponInHands->OnFireEvent.AddDynamic(this, &AQuarantineProjectCharacter::ShowFireAnimation);
		// set appropriate collision settings to avoid collision with player
		WeaponInHands->SetMeshCollision(ECollisionResponse::ECR_Overlap);
		// set aiming camera field of view depends on weapon
		AimingCamera->FieldOfView = WeaponInHands->GetAimingFieldOfView();
		// disable aiming for new weapon if it exists
		if (bIsAiming)
		{
			AimToTarget();
		}
		if (!bIsWeaponInHands)
		{
			bIsWeaponInHands = true;
		}
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
	if (true && (Value != 0.0f))//(Controller != NULL) 
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
	if (true  && (Value != 0.0f) )//(Controller != NULL)
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

void AQuarantineProjectCharacter::AddControllerYawInput(float Value)
{
	Super::AddControllerYawInput(Value);
}

void AQuarantineProjectCharacter::AddControllerPitchInput(float Value)
{
	Super::AddControllerPitchInput(Value);
}

void AQuarantineProjectCharacter::StartCrouching()
{
	Super::Crouch();
}

void AQuarantineProjectCharacter::StopCrouching()
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

void AQuarantineProjectCharacter::HideWeapon()
{
	if (WeaponInHands)
	{
		WeaponInHands->Destroy();
		bIsWeaponInHands = false;
	}
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
		if (WeaponInHands && bIsWeaponInHands)
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
			GetPlayerHUD()->UpdateHealthState(HealthComponent->GetCurrentHealth() / 
												HealthComponent->GetDefaultHealth());
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
