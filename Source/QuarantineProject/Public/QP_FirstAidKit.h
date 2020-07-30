// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QP_FirstAidKit.generated.h"

UCLASS()
class QUARANTINEPROJECT_API AQP_FirstAidKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQP_FirstAidKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// components
	UPROPERTY(EditAnywhere, Category = "Aid Components")
	class UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, Category = "Aid Components")
	class UStaticMeshComponent* AidMesh;
	// how mush health player get after picking up aid kit
	UPROPERTY(EditAnywhere, Category = "Aid Properties")
	float HealthValue = 10.0;
};
