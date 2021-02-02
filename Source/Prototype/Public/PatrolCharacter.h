// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolCharacter.generated.h"


class AWaypoint;


UENUM(BlueprintType)
enum class PatrolMode : uint8
{
	PATROL UMETA(DisplayName = "PATROL"),
	CHASE UMETA(DisplayName = "CHASE")
};


UCLASS()
class PROTOTYPE_API APatrolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APatrolCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		PatrolMode Mode = PatrolMode::PATROL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PatrolSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChaseSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AWaypoint*> Waypoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentWaypointIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceBeforeArrivedAtWaypoint = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WaypointDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceBeforeArrivedAtPlayer = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAtWaypoint = false;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPatrolMode();

	void SetChaseMode();
};
