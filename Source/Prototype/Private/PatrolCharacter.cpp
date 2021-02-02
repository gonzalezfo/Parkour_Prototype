// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APatrolCharacter::APatrolCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set the default rotation rate and the max walking speed.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	SetPatrolMode();
}


void APatrolCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void APatrolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void APatrolCharacter::SetPatrolMode()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	Mode = PatrolMode::PATROL;
}


void APatrolCharacter::SetChaseMode()
{
	bIsAtWaypoint = false;

	if (GetCharacterMovement() == nullptr) return;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	Mode = PatrolMode::CHASE;
}

