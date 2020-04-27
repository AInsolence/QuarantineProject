// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "QP_HealthComponent.h"
#include "QuarantineProject/Public/QP_WeaponBase.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Prevent capsule to be hitted with projectile
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 340.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Create health component
	HealthComponent = CreateDefaultSubobject<UQP_HealthComponent>(TEXT("HealthComponent"));
	// Create AI perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

//////////////////////////////////////////////////////////////////////////

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true; // allow to move camera up'n'down TRUE
	bUseControllerRotationRoll = false;

	// set base speed sprint variables
	BaseWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// On take damage subscribing
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBaseCharacter::OnTakeDamage);

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
		WeaponInHands->OnReloading.AddDynamic(this, &AEnemyBaseCharacter::ShowReloadAnimation);
	}

}

void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/**  Sprint logic */
	float RampThisFrame = (DeltaTime / TimeToMaxSprintSpeed) * MaxSprintMultiplier;
	if (bIsSprinting && HealthComponent->GetCurrentStamina() > 0.5f)
	{
		BaseSprintMultiplier += RampThisFrame;
	}
	else
	{
		BaseSprintMultiplier -= RampThisFrame;
	}
	BaseSprintMultiplier = FMath::Clamp(BaseSprintMultiplier, 1.0f, MaxSprintMultiplier);
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkingSpeed * BaseSprintMultiplier;
	/**  Sprint logic end */
}

void AEnemyBaseCharacter::ShowReloadAnimation()
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

//*****            INPUT LOGIC                *****//

void AEnemyBaseCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AEnemyBaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AEnemyBaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AEnemyBaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEnemyBaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEnemyBaseCharacter::MoveForward(float Value)
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

void AEnemyBaseCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
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

void AEnemyBaseCharacter::Jump()
{
	Super::Jump();
}

void AEnemyBaseCharacter::StopJumping()
{
	Super::StopJumping();
}

void AEnemyBaseCharacter::AddControllerYawInput(float Value)
{
	Super::AddControllerYawInput(Value);
}

void AEnemyBaseCharacter::AddControllerPitchInput(float Value)
{
	Super::AddControllerPitchInput(Value);
}

void AEnemyBaseCharacter::Crouch()
{
	Super::Crouch();
}

void AEnemyBaseCharacter::UnCrouch()
{
	Super::UnCrouch();
}

void AEnemyBaseCharacter::SprintStart()
{
	bIsSprinting = true;
}

void AEnemyBaseCharacter::SprintEnd()
{
	bIsSprinting = false;
}

void AEnemyBaseCharacter::AimToTarget()
{// Switch aiming status
	bIsAiming = !bIsAiming;
	// TODO make aiming logic for NPC here

}

void AEnemyBaseCharacter::OnFire()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (WeaponInHands)
		{
			FRotator MuzzleRotation;
			MuzzleRotation = WeaponInHands->GetMuzzleRotation();
			WeaponInHands->Fire(MuzzleRotation);
		}
	}

	// try and play a firing animation if specified
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

//*****                                        *****//

void AEnemyBaseCharacter::OnTakeDamage(AActor* DamagedActor,
										float Damage,
										const UDamageType* DamageType,
										AController* InstigatedBy,
										AActor* DamageCauser)
{
	if (HealthComponent)
	{
		//*** DEATH ***//
		if (HealthComponent->GetCurrentHealth() <= 0)
		{
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
				// unpossess to stop AI
				CurrentController->UnPossess();
				// destroy the controller, since it's not part of the enemy anymore
				CurrentController->Destroy();
			}
		}
		// *** DEATH END *** //
	}

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
}
