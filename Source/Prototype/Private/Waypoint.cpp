// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"
#include "Components/BoxComponent.h"
#include "PatrolCharacter.h"

// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnWaypointBeginOverlap);
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypoint::OnWaypointBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	APatrolCharacter* PatrolCharacter = Cast<APatrolCharacter>(OtherActor);
	if (PatrolCharacter == nullptr) return;

	PatrolCharacter->bIsAtWaypoint = true;
}
