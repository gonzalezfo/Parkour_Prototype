// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedKit.generated.h"

class AMyPlayer;
class UBoxComponent;



UCLASS()
class PROTOTYPE_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKit();

	UPROPERTY(EditAnywhere)
		UBoxComponent* MyBoxComponent;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> Regeneration;

	UPROPERTY(EditAnywhere)
		FHitResult MyHit;

	UFUNCTION()
		//void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
		AMyPlayer* MyPlayer;

	UPROPERTY(EditAnywhere, Category = "Health")
		float recoverAmount;

};
