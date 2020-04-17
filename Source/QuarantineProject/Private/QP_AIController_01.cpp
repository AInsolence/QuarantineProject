// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_AIController_01.h"
#include "QuarantineProject/QuarantineProjectCharacter.h"
#include "TimerManager.h"

void AQP_AIController_01::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AQP_AIController_01::OnFire, 1.0f, true);
}

void AQP_AIController_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACharacter* AQP_AIController_01::GetControlledCharacter() const
{
	return GetCharacter();
}

void AQP_AIController_01::MoveForward(float Value)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->MoveForward(Value);
}

void AQP_AIController_01::MoveRight(float Value)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->MoveRight(Value);
}

void AQP_AIController_01::AddControllerYawInput(float Value)
{
	GetControlledCharacter()->AddControllerYawInput(Value);
}

void AQP_AIController_01::AddControllerPitchInput(float Value)
{
	GetControlledCharacter()->AddControllerPitchInput(Value);
}

void AQP_AIController_01::Jump()
{
	GetControlledCharacter()->Jump();
}

void AQP_AIController_01::StopJumping()
{
	GetControlledCharacter()->StopJumping();
}

void AQP_AIController_01::Crouch()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->Crouch();
}

void AQP_AIController_01::UnCrouch()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->UnCrouch();
}

void AQP_AIController_01::SprintStart()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->SprintStart();
}

void AQP_AIController_01::SprintEnd()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->SprintEnd();
}

void AQP_AIController_01::AimToTarget()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->AimToTarget();
}

void AQP_AIController_01::OnFire()
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->OnFire();
}

void AQP_AIController_01::TurnAtRate(float Rate)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->TurnAtRate(Rate);
}

void AQP_AIController_01::LookUpAtRate(float Rate)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->LookUpAtRate(Rate);
}

void AQP_AIController_01::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
		TouchStarted(FingerIndex, Location);
}

void AQP_AIController_01::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AQuarantineProjectCharacter>(GetControlledCharacter())->
		TouchStopped(FingerIndex, Location);
}
