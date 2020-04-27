// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleProjectile_01.generated.h"

UCLASS()
class QUARANTINEPROJECT_API ARifleProjectile_01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleProjectile_01();

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	/** Explosion effect */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UParticleSystem* ExplosionParticles = nullptr;

	// Set damage value
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float DamageAmount = 30.0f;

	/** Explosion effect */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ACharacter> InflictDamageToThisClass;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	// explosive status
	bool bIsExplosiveRound = true;

};
