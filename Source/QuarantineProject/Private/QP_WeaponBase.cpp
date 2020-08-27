// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RifleProjectile_01.h"
#include "Inventory/QP_PickableComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AQP_WeaponBase::AQP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	PickableComponent = CreateDefaultSubobject<UQP_PickableComponent>(TEXT("PickableComponent"));

	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AQP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();

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

void AQP_WeaponBase::SetMeshCollision(ECollisionResponse Response)
{
	WeaponMesh->SetCollisionResponseToAllChannels(Response);
}

float AQP_WeaponBase::GetAimingFieldOfView()
{
	return AimingFealdOfView;
}

void AQP_WeaponBase::Fire(FRotator MuzzleRotation)
{
	if (!TotalBulletsForThisWeapon && !CurrentBulletsInMagazine)
	{
		return;
	}
	if (bIsWeaponReloading) return;

	if(!bCanFireAfterRate)
	{// wait for delay after last firing time to avoid hypertapping cheat
		bCanFireAfterRate = ((FPlatformTime::Seconds() - LastFireTime) > ShotRate);
		if (bCanFireAfterRate)
		{
			LastFireTime = FPlatformTime::Seconds();
		}
	}

	if (bCanFireAfterRate)
	{// start fire
		bCanFireAfterRate = false;
		// start fire without delay
		FireLoop(MuzzleRotation);

		// start fire loop by timer if still pressed
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				FireRateTimer,
				FTimerDelegate::CreateUObject(this, &AQP_WeaponBase::FireLoop, MuzzleRotation),
				ShotRate,
				true);
		}
	}
}

void AQP_WeaponBase::FireLoop(FRotator MuzzleRotation)
{// check bullets in magazine before spawning projectile
	if (CurrentBulletsInMagazine > 0)
	{
		SpawnProjectile(MuzzleRotation);
		CurrentBulletsInMagazine -= 1;
	}
	else // try to reload
	{
		StopFiring();
		StartReload();
	}
	GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red, TEXT("Bullets: %f") + FString::FromInt(CurrentBulletsInMagazine), false);
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
				FRotator FireRotator;
				FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
				//auto CameraRotation = UGameplayStatics::GetPlayerCameraManager(this, 0)->
				//								GetActorForwardVector().ToOrientationRotator();
				//// Deproject aim to world point
				//auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				//if (PlayerController)
				//{
				//	FIntPoint ScreenSize;
				//	PlayerController->GetViewportSize(ScreenSize.X, ScreenSize.Y);
				//	FVector2D ScreenCenter = (ScreenSize.X/2.0f, ScreenSize.Y / 2.0f);
				//	UE_LOG(LogTemp, Warning, TEXT("Direction deproject point: %s"), *ScreenCenter.ToString());
				//	FVector OutWorldLocation;
				//	FVector OutWorldDirection;
				//	// Find world point
				//	PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, 
				//														ScreenCenter.Y, 
				//														OutWorldLocation, 
				//														OutWorldDirection);
				auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (CameraManager)
				{
					MuzzleRotation = CameraManager->GetActorForwardVector().Rotation();
				}
				// Broadcast OnFireEvent
				OnFireEvent.Broadcast();
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
						MuzzleLocation,
						MuzzleRotation);
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

void AQP_WeaponBase::StopFiring()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
	}
}

void AQP_WeaponBase::StartReload()
{
	// play the empty magazine sound
	if (OutOfBulletsSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OutOfBulletsSound, GetActorLocation());
	}
	// check if any additional ammo exist for reloading
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

void AQP_WeaponBase::Reload()
{
	GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Red, TEXT("Reloading") + FString::FromInt(TotalBulletsForThisWeapon), false);
	// check if some bullets in mag
	int32 RemainBullets = CurrentBulletsInMagazine;
	// update current bullets value
	CurrentBulletsInMagazine = FMath::Clamp(TotalBulletsForThisWeapon - BulletsInMagazine + RemainBullets,
											0, 
											BulletsInMagazine);
	// update total bullets value
	TotalBulletsForThisWeapon = FMath::Clamp(TotalBulletsForThisWeapon - BulletsInMagazine + RemainBullets,
											0,
											120);
	UE_LOG(LogTemp, Warning, TEXT("Remain Bullets: %d"), RemainBullets)
	UE_LOG(LogTemp, Warning, TEXT("TOTAL Bullets: %d"), TotalBulletsForThisWeapon)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	}
	// weapon now can be reloaded
	bIsWeaponReloading = false;
}

void AQP_WeaponBase::Reloading()
{
	if (bIsWeaponReloading)
	{
		return;
	}
	if (CurrentBulletsInMagazine != BulletsInMagazine)
	{
		StartReload();
	}
}
