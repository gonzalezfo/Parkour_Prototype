// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableWall.h"
#include "Components/BoxComponent.h"
#include "../Public/MyPlayer.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARunnableWall::ARunnableWall()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	this->SetRootComponent(CubeMesh);

	CollisionMeshStart = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh Start"));
	CollisionMeshStart->SetupAttachment(CubeMesh);	
	
	CollisionMeshFinish = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh Finish"));
	CollisionMeshFinish->SetupAttachment(CubeMesh);

	CollisionMeshStart->OnComponentBeginOverlap.AddDynamic(this, &ARunnableWall::OnOverlapBeginWithStart);
	CollisionMeshStart->OnComponentEndOverlap.AddDynamic(this, &ARunnableWall::OnOverlapEndWithStart);
	CollisionMeshFinish->OnComponentBeginOverlap.AddDynamic(this, &ARunnableWall::OnOverlapBeginWithEnd);

}

// Called when the game starts or when spawned
void ARunnableWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunnableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARunnableWall::OnOverlapBeginWithStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		MyPlayer->SetRunnableWall_Implementation(CollisionMeshFinish->GetComponentLocation(), CollisionMeshStart->GetComponentTransform(), true);
		UE_LOG(LogTemp, Warning, TEXT("Entro"));
	}
}

void ARunnableWall::OnOverlapEndWithStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		MyPlayer->SetRunnableWall_Implementation(CollisionMeshFinish->GetComponentLocation(), CollisionMeshStart->GetComponentTransform(), false);
		UE_LOG(LogTemp, Warning, TEXT("Salgo"));

	}
}

void ARunnableWall::OnOverlapBeginWithEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		MyPlayer->StopAnimMontage();
		UE_LOG(LogTemp, Warning, TEXT("Acabao"));
	}
}

