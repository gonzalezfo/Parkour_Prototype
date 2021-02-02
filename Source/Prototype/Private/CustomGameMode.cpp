// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CustomGameMode.h"
#include "../Public/MyPlayer.h"
#include "../Public/ACustomHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ACustomGameMode::ACustomGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_MyPlayer.BP_MyPlayer_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;


	HUDClass = AACustomHUD::StaticClass();
}

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	MyPlayer_ = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ACustomGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetWorld()->GetMapName();

	if (MyPlayer_)
	{
		if (FMath::IsNearlyZero(MyPlayer_->GetHealth(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}
	}
}

EGamePlayState ACustomGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ACustomGameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ACustomGameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
	{
		// do nothing
	}
	break;
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	break;
	// Unknown/default state
	default:
	case EGamePlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}