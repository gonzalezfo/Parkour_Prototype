// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Trap.h"
#include "../Public/MyPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ATrap::ATrap()
{
	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = MyBoxComponent;

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlapBegin);
	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATrap::OnOverlapEnd);

	bCanApplyDamage = false;

	DamageAmount = 200.0f;
}

void ATrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		bCanApplyDamage = true;
		MyCharacter = Cast<AMyPlayer>(OtherActor);
		MyHit = SweepResult;
		GetWorldTimerManager().SetTimer(SpikeTimerHandle, this, &ATrap::ApplySpikeDamage, 2.2f, true, 0.0f);
	}
}

void ATrap::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(SpikeTimerHandle);
}

void ATrap::ApplySpikeDamage()
{
	if (bCanApplyDamage)
	{
		UGameplayStatics::ApplyPointDamage(MyCharacter, DamageAmount, GetActorLocation(), MyHit, nullptr, this, SpikeDamage);
	}
}
