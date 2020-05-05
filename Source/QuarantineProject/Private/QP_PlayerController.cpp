// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_PlayerController.h"
#include "QuarantineProject/QuarantineProjectCharacter.h"
#include "QP_HUD.h"
#include "QP_InventorySystemComponent.h"

void AQP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GetCharacter() && GetHUD())
	{
		// set binding to show Game over widget
		Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
			OnGameOverByPlayersDeath.AddDynamic(this, &AQP_PlayerController::GameOver);
		// set binding to show pick up tip
		auto HUD = Cast<AQP_HUD>(GetHUD());
		auto Inventory = Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
			FindComponentByClass<UQP_InventorySystemComponent>();
		if (Inventory)
		{
			Inventory->OnItemCanBePickedUp.BindDynamic(HUD, &AQP_HUD::SetPickUpTipVisibility);
		}
	}
}

void AQP_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACharacter* AQP_PlayerController::GetControlledCharacter() const
{
	return GetCharacter();
}

void AQP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Set up gameplay key bindings
	InputComponent->BindAction("Exit", IE_Pressed, this, &AQP_PlayerController::Exit);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AQP_PlayerController::OnFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AQP_PlayerController::OnStopFiring);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AQP_PlayerController::OnReloading);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AQP_PlayerController::Interact);
	InputComponent->BindAction("Drop", IE_Pressed, this, &AQP_PlayerController::Drop);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AQP_PlayerController::SprintStart);
	InputComponent->BindAction("Sprint", IE_Released, this, &AQP_PlayerController::SprintEnd);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AQP_PlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AQP_PlayerController::StopJumping);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AQP_PlayerController::Crouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &AQP_PlayerController::UnCrouch);

	InputComponent->BindAction("Aim", IE_Pressed, this, &AQP_PlayerController::AimToTarget);

	InputComponent->BindAxis("MoveForward", this, &AQP_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AQP_PlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &AQP_PlayerController::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AQP_PlayerController::AddControllerPitchInput);
	InputComponent->BindAxis("TurnRate", this, &AQP_PlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &AQP_PlayerController::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AQP_PlayerController::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AQP_PlayerController::TouchStopped);
}

void AQP_PlayerController::MoveForward(float Value)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->MoveForward(Value);
}

void AQP_PlayerController::MoveRight(float Value)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->MoveRight(Value);
}

void AQP_PlayerController::AddControllerYawInput(float Value)
{
	GetControlledCharacter()->AddControllerYawInput(Value);
}

void AQP_PlayerController::AddControllerPitchInput(float Value)
{
	GetControlledCharacter()->AddControllerPitchInput(Value);
}

void AQP_PlayerController::Jump()
{
	GetControlledCharacter()->Jump();
}

void AQP_PlayerController::StopJumping()
{
	GetControlledCharacter()->StopJumping();
}

void AQP_PlayerController::Crouch()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->Crouch();
}

void AQP_PlayerController::UnCrouch()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->UnCrouch();
}

void AQP_PlayerController::SprintStart()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->SprintStart();
}

void AQP_PlayerController::SprintEnd()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->SprintEnd();
}

void AQP_PlayerController::AimToTarget()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->AimToTarget();
}

void AQP_PlayerController::OnFire()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->OnFire();
}

void AQP_PlayerController::OnStopFiring()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->OnStopFiring();
}

void AQP_PlayerController::OnReloading()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->OnReloading();
}

void AQP_PlayerController::Exit()
{
	Cast<AQP_HUD>(GetHUD())->Exit();
}

void AQP_PlayerController::GameOver()
{
	if (GetHUD())
	{
		Cast<AQP_HUD>(GetHUD())->GameOver();
	}
}

void AQP_PlayerController::Interact()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->PickUpItem();
}

void AQP_PlayerController::Drop()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->DropItem();
}

void AQP_PlayerController::TurnAtRate(float Rate)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->TurnAtRate(Rate);
}

void AQP_PlayerController::LookUpAtRate(float Rate)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->LookUpAtRate(Rate);
}

void AQP_PlayerController::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
		TouchStarted(FingerIndex, Location);
}

void AQP_PlayerController::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
		TouchStopped(FingerIndex, Location);
}
