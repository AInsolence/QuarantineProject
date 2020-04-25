// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QP_WeaponBase.generated.h"

UENUM()
enum class EWeaponType
{
	ColdSteel,
	Pistol,
	Rifle,
	SniperRifle,
	Turret,
	Grenade
};

UENUM()
enum class EFireMode
{
	SemiAuto,
	Burst,
	Auto
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadingEvent);

UCLASS()
class QUARANTINEPROJECT_API AQP_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQP_WeaponBase();
	// API
	UFUNCTION()
	void Fire(FRotator MuzzleRotation);
	UFUNCTION()
	void Reload();
	UFUNCTION()
	FRotator GetMuzzleRotation();
	// Is weapon have any ammo
	bool FORCEINLINE IsWeaponCanShoot() { return CurrentBulletsInMagazine != 0; };
	// Get reloading event
	FReloadingEvent OnReloading;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponents")
	class UBoxComponent* BoxCollision = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponents")
	class USkeletalMeshComponent* WeaponMesh = nullptr;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponProperties")
	class USoundBase* FireSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponProperties")
	class USoundBase* ReloadSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponProperties")
	class USoundBase* OutOfBulletsSound = nullptr;
	/** Fire particle effects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponProperties")
	class UParticleSystem* MuzzleFireEffect = nullptr;
	/** Crosshair widget for this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	TSubclassOf<class UAimingWidget> CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	EWeaponType WeaponType = EWeaponType::Rifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	int32 BulletsInMagazine = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	int32 CurrentBulletsInMagazine = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	int32 TotalBulletsForThisWeapon = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	EFireMode FireMode = EFireMode::Auto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	float AimingFealdOfView = 80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	float ReloadingTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperties")
	float ShotsPerSecond = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ARifleProjectile_01> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ARifleProjectile_01> Shell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage = 10.0f;
private:
	void SpawnProjectile(FRotator MuzzleRotation);
	FTimerHandle ReloadTimer;
	bool bIsWeaponReloading = false;
};
