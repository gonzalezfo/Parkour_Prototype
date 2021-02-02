// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PatrolAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APatrolAIController : public AAIController
{
	GENERATED_BODY()

public:
	APatrolAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UFUNCTION()
		void MoveToWaypoint();

	UPROPERTY()
		FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector PositionToMoveTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class APatrolCharacter* PatrolCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AIHearingRange = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AIHearingMaxAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bAIHearingStartsEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float DistanceToPlayer = 0.0f;	
};
