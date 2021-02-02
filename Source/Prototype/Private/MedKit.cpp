// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "../Public/MyPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AMedKit::AMedKit()
{
	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = MyBoxComponent;

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMedKit::OnOverlapBegin);
	recoverAmount = 100.0f;
}


void AMedKit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		MyHit = SweepResult;
		UGameplayStatics::ApplyPointDamage(MyPlayer, -recoverAmount, GetActorLocation(), MyHit, nullptr, this, Regeneration);
		Destroy();
	}
}



