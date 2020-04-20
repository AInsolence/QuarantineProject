// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "QuarantineProject/Public/RifleProjectile_01.h"

// Sets default values
AQP_WeaponBase::AQP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//BoxCollision->SetBoxExtent(FVector(2, 44, 14));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetRootComponent(WeaponMesh);
	WeaponMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//WeaponMesh->SetRelativeLocation(FVector(0, -17, 0));
}

void AQP_WeaponBase::Fire(FRotator MuzzleRotation)
{
	// try and fire a projectile
	if (Projectile != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (WeaponMesh)
			{
				FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				// spawn the projectile at the place
				World->SpawnActor<ARifleProjectile_01>(Projectile,
					MuzzleLocation,
					MuzzleRotation,
					ActorSpawnParams);
				// spawn the projectiles shell
				World->SpawnActor<ARifleProjectile_01>(Shell,
					WeaponMesh->GetSocketLocation("ShellSocket"),
					WeaponMesh->GetSocketRotation("ShellSocket"),
					ActorSpawnParams);
				// spawn muzzle fire effect
				if (MuzzleFireEffect)
				{
					auto Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						MuzzleFireEffect,
						MuzzleLocation);
					Explosion->SetRelativeScale3D(FVector(0.03f));
				}

			}
		}
	}
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

FRotator AQP_WeaponBase::GetMuzzleRotation()
{
	if (WeaponMesh)
	{
		return WeaponMesh->GetSocketRotation("Muzzle");
	}
	else
	{// return if socket is not specified
		UE_LOG(LogTemp, Warning, TEXT(" QP_WeaponBase class: Muzzle socket is not specified."))
		return GetActorRotation();
	}
}

// Called when the game starts or when spawned
void AQP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}
