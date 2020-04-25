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
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AQP_FirstAidKit::OnPickedUp);
}

void AQP_FirstAidKit::OnPickedUp(UPrimitiveComponent* OverlappedComponent, 
								AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, 
								int32 OtherBodyIndex, 
								bool bFromSweep, 
								const FHitResult& SweepResult)
{
	if (OtherActor == //if overlapped with player
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		// Create a damage event  
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		// Take negative damage to heal
		OtherActor->TakeDamage(HealthValue * (-1),
								DamageEvent,
								GetWorld()->GetFirstPlayerController(),
								this);
		Destroy();
	}
}
