// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleProjectile_01.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "QuarantineProject/QuarantineProjectCharacter.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"

// Sets default values
ARifleProjectile_01::ARifleProjectile_01()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ARifleProjectile_01::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ARifleProjectile_01::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsExplosiveRound && ExplosionParticles)
	{
		// create explosion particle effect
		auto Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());
		//Explosion->SetRelativeScale3D(FVector(0.3f));
		if (InflictDamageToThisClass)
		{
			if (OtherActor->GetClass()->IsChildOf(InflictDamageToThisClass))
			{
				// Create a damage event  
				TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
				FDamageEvent DamageEvent(ValidDamageTypeClass);
				// change damage amount in correspondence to hitted bone
				FName BoneName = Hit.BoneName;
				if (BoneName == FName("head"))
				{
					DamageAmount *= 5;
				}
				else if (BoneName == FName("neck_01"))
				{
					DamageAmount *= 3;
				}
				else if (BoneName == FName("spine_03"))
				{
					DamageAmount *= 2;
				}

				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Black, TEXT("Hitted bone: ") + BoneName.ToString(), false);
				GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Black, TEXT("DAMAGE: %f") + FString::SanitizeFloat(DamageAmount), false);
				if (GetWorld())
				{
					if (GetWorld()->GetFirstPlayerController())
					{
						OtherActor->TakeDamage(DamageAmount,
							DamageEvent,
							GetWorld()->GetFirstPlayerController(),
							this);
					}
				}
			}
		}
		// destroy projectile
		Destroy();
	}
}
