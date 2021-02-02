// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolAIController.h"
#include "Waypoint.h"
#include "PatrolCharacter.h"
#include "MyPlayer.h"
#include "Tasks/AITask_MoveTo.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine.h"


APatrolAIController::APatrolAIController()
{
	PrimaryActorTick.bCanEverTick = true;


	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	HearingConfig->HearingRange = AIHearingRange;
	HearingConfig->LoSHearingRange = AIHearingRange;
	HearingConfig->SetMaxAge(AIHearingMaxAge);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}


void APatrolAIController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &APatrolAIController::OnPawnDetected);

	PatrolCharacter = Cast<APatrolCharacter>(GetPawn());


	PatrolCharacter->CurrentWaypointIndex = PatrolCharacter->Waypoints.Num() - 1;

	MoveToWaypoint();
}


void APatrolAIController::OnPossess(APawn* PatrolPawn)
{
	Super::OnPossess(PatrolPawn);
}


void APatrolAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


FRotator APatrolAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr) return FRotator(0.0f, 0.0f, 0.0f);

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}


void APatrolAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{

	for (AActor* DetectedPawn : DetectedPawns)
	{

		if (!HearingConfig->GetSenseID().IsValid()) return;
		const FActorPerceptionInfo* HeardPerceptionInfo = GetPerceptionComponent()->GetFreshestTrace(HearingConfig->GetSenseID());

		if (!HeardPerceptionInfo->IsSenseActive(HearingConfig->GetSenseID()))
		{

			MoveToWaypoint();
			return;
		}

		AMyPlayer* PlayerCharacter = Cast<AMyPlayer>(DetectedPawn);
		if (PlayerCharacter == nullptr) return;

		bIsPlayerDetected = true;
		DistanceToPlayer = GetPawn()->GetDistanceTo(PlayerCharacter);

		PositionToMoveTo = HeardPerceptionInfo->GetStimulusLocation(HearingConfig->GetSenseID());

		PatrolCharacter->SetChaseMode();

		MoveToLocation(PositionToMoveTo, PatrolCharacter->DistanceBeforeArrivedAtPlayer);

	}
}


void APatrolAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (PatrolCharacter->Mode == PatrolMode::PATROL)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APatrolAIController::MoveToWaypoint, PatrolCharacter->WaypointDelay);
	}
}


void APatrolAIController::MoveToWaypoint()
{

	if (PatrolCharacter->Mode == PatrolMode::CHASE)
	{
		PatrolCharacter->SetPatrolMode();
	}
	else
	{
		if (PatrolCharacter->CurrentWaypointIndex == PatrolCharacter->Waypoints.Num() - 1)
		{
			PatrolCharacter->CurrentWaypointIndex = 0;
		}
		else
		{
			PatrolCharacter->CurrentWaypointIndex++;
		}
	}


	MoveToActor(PatrolCharacter->Waypoints[PatrolCharacter->CurrentWaypointIndex], PatrolCharacter->DistanceBeforeArrivedAtWaypoint);

	PatrolCharacter->bIsAtWaypoint = false;
}
