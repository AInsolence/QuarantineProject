// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "QuarantineProject/Public/RifleProjectile_01.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

// Sets default values
AQP_WeaponBase::AQP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetRootComponent(WeaponMesh);
}

void AQP_WeaponBase::Fire(FRotator MuzzleRotation)
{
	if (CurrentBulletsInMagazine)
	{
		// try to fire a projectile
		SpawnProjectile(MuzzleRotation);
		CurrentBulletsInMagazine -= 1;
	}
	else
	{
		// try and play the sound if specified
		if (OutOfBulletsSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OutOfBulletsSound, GetActorLocation());
		}
		// if any additional ammo exist
		if ((TotalBulletsForThisWeapon - BulletsInMagazine) > 0)
		{// if weapon has ammo
			OnReloading.Broadcast();
			// play reload sound
			if (ReloadSound != NULL && !bIsWeaponReloading)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
			}
			bIsWeaponReloading = true;
			// waiting for reloading time and reload
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimer, 
														this, 
														&AQP_WeaponBase::Reload, 
														ReloadingTime, 
														false);
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red, TEXT("Bullets: %f") + FString::FromInt(CurrentBulletsInMagazine), false);
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

void AQP_WeaponBase::SpawnProjectile(FRotator MuzzleRotation)
{
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

void AQP_WeaponBase::Reload()
{
	GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Red, TEXT("Reloading") + FString::FromInt(TotalBulletsForThisWeapon), false);
	// update current bullets value
	CurrentBulletsInMagazine = FMath::Clamp(TotalBulletsForThisWeapon - BulletsInMagazine,
											0, 
											BulletsInMagazine);
	// update total bullets value
	TotalBulletsForThisWeapon = FMath::Clamp(TotalBulletsForThisWeapon - BulletsInMagazine,
											0,
											120);
	UE_LOG(LogTemp, Warning, TEXT("TOTAL Bullets: %d"), TotalBulletsForThisWeapon)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	}
	// weapon now can be reloaded
	bIsWeaponReloading = false;
}
