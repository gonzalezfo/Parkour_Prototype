// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "WallRunner.h"
#include "MyPlayer.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UCurveFloat;
class UAnimMontage;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FHandleDamageSignature, AMyPlayer*, myPlayer, float, HealthPercentage, struct FDamageEvent const&, DamageEvent, class AController*, EventInstigator, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegenerateStamina);


UCLASS(config=Game)
class PROTOTYPE_API AMyPlayer : public ACharacter, public IWallRunner
{
	GENERATED_BODY()

public:
	AMyPlayer();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void BeginCrouch(float value);
	void Dash(float value);
	void EndCrouch();
	void BeginJump();
	void EndJump();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp_;



public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Health variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float PreviousHealth;

	bool bCanBeDamaged;

	// Stamina variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float FullStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float PreviousStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float DashModifier;

	bool bCanUseStamina;

	bool bHasLostStamina;

	bool bCanWallRun;

	FVector target_;
	FTransform startPoint_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
		UCurveFloat* StaminaCurve;

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle MemberTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* WallRunMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UPawnNoiseEmitterComponent* PlayerNoiseEmitter;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthChange);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void UpdateStamina(float StaminaChange);

	UFUNCTION()
		void DamageTimer();

	UFUNCTION()
		void SetDamageState();

	UFUNCTION()
		void RegenerateStamina();

	UFUNCTION()
		void SetStaminaValue();

	UFUNCTION()
		void StopWallRunAnim();

	UPROPERTY(BlueprintAssignable, Category = "Health")
		FHandleDamageSignature OnHandleAnyDamage;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
		FOnRegenerateStamina OnRegenerateStamina;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void SetRunnableWall(FVector target, FTransform startPoint, bool enable);
		void SetRunnableWall_Implementation(FVector target, FTransform startPoint, bool enable);

};
