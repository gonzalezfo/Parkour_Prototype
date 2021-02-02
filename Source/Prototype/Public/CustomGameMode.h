// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

class AMyPlayer;

UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};


UCLASS()
class PROTOTYPE_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACustomGameMode();

		virtual void BeginPlay() override;
	
		virtual void Tick(float DeltaTime) override;
	
		AMyPlayer* MyPlayer_;
	
		// Return current player state
		UFUNCTION(BlueprintPure, Category = "Health")
			EGamePlayState GetCurrentState() const;
	
		// Sets player state
		void SetCurrentState(EGamePlayState NewState);
	
	private:
		EGamePlayState CurrentState;
	
		void HandleNewState(EGamePlayState NewState);
};
