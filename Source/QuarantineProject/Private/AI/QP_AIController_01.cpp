// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QP_AIController_01.h"
#include "AI/EnemyBaseCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

void AQP_AIController_01::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AQP_AIController_01::OnFire, 1.0f, true);
}

void AQP_AIController_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*auto const World = GetWorld();

	if (World)
	{
		auto const Controller = World->GetFirstPlayerController();

		if (Controller)
		{
			auto PlayerCharacter = Controller->GetPawn();
			if (PlayerCharacter)
			{
				MoveToActor(PlayerCharacter, 1500, true, true, false);
			}
				
		}
	}*/
}

ACharacter* AQP_AIController_01::GetControlledCharacter() const
{
	return GetCharacter();
}

void AQP_AIController_01::MoveForward(float Value)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->MoveForward(Value);
}

void AQP_AIController_01::MoveRight(float Value)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->MoveRight(Value);
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
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->StartCrouching();
}

void AQP_AIController_01::UnCrouch()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->StopCrouching();
}

void AQP_AIController_01::SprintStart()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->SprintStart();
}

void AQP_AIController_01::SprintEnd()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->SprintEnd();
}

void AQP_AIController_01::AimToTarget()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->AimToTarget();
}

void AQP_AIController_01::OnFire()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->OnFire();
}

void AQP_AIController_01::OnStopFiring()
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->OnStopFiring();
}

void AQP_AIController_01::TurnAtRate(float Rate)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->TurnAtRate(Rate);
}

void AQP_AIController_01::LookUpAtRate(float Rate)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->LookUpAtRate(Rate);
}

void AQP_AIController_01::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->
		TouchStarted(FingerIndex, Location);
}

void AQP_AIController_01::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	Cast<AEnemyBaseCharacter>(GetControlledCharacter())->
		TouchStopped(FingerIndex, Location);
}
