// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_FirstAidKit.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
AQP_FirstAidKit::AQP_FirstAidKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->SetSimulatePhysics(false);
	
	AidMesh = CreateDefaultSubobject<UStaticMeshComponent>("AidMesh");
	if (AidMesh)
	{
		AidMesh->SetupAttachment(BoxCollision);
	}
}

// Called when the game starts or when spawned
void AQP_FirstAidKit::BeginPlay()
{
	Super::BeginPlay();
}
