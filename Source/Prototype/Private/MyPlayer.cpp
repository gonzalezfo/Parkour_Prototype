// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/MyPlayer.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/EngineTypes.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"


AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tmp(TEXT("/Game/Art/Character/WallRun_Montage.WallRun_Montage"));
	WallRunMontage = tmp.Object;

	SpringArmComp_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp_->SetupAttachment(RootComponent);
	SpringArmComp_->bUsePawnControlRotation = true;


	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp_ = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp_->SetupAttachment(SpringArmComp_);

	PlayerNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	PlayerNoiseEmitter->SetAutoActivate(true);

	// Value initialization
	FullHealth = 1000.0f;
	Health = FullHealth;
	HealthPercentage = 1.0f;
	PreviousHealth = HealthPercentage;
	bCanBeDamaged = true;

	FullStamina = 100.0f;
	Stamina = FullStamina;
	StaminaPercentage = 1.0f;
	PreviousStamina = StaminaPercentage;
	StaminaValue = 0.0f;
	bCanUseStamina = true;
	bHasLostStamina = false;
	bCanWallRun = false;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (StaminaCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetStaminaValue"));
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("SetStaminaState") });
		MyTimeline.AddInterpFloat(StaminaCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);

	RegenerateStamina();

	PlayerNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
}

void AMyPlayer::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void AMyPlayer::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void AMyPlayer::BeginCrouch(float value)
{
	if (value == 1.0f)
	{
		if (bCanUseStamina) {
			if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
			{
				FVector tmp = GetVelocity();
				if (tmp.Size() > 0.0f) {
					tmp.Normalize();
					if (FVector::DotProduct(GetActorForwardVector(), tmp) > 0.0f) {
						if (Stamina > 1.0f) {
							Crouch();
							UpdateStamina(-1.0f);
							OnRegenerateStamina.Broadcast();
							bHasLostStamina = true;
							this->GetCharacterMovement()->Velocity += FVector(GetActorForwardVector() * (DashModifier * 0.5f) * GetWorld()->DeltaTimeSeconds);
						}
						else {
							EndCrouch();
						}
					}
				}
			}
		}
		else {
			EndCrouch();
		}
	} else {
		EndCrouch();
	}
}

void AMyPlayer::Dash(float value)
{
	if (value == 1.0f) {
		if (bCanUseStamina) 
		{
			if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
			{
				FVector tmp = GetVelocity();
				if (tmp.Size() > 0.0f) {
					tmp.Normalize();
					if (FVector::DotProduct(GetActorForwardVector(), tmp) > 0.0f) {
						if (Stamina > 0.0f) {
							UpdateStamina(-0.5f);
							bHasLostStamina = true;
							OnRegenerateStamina.Broadcast();
							this->GetCharacterMovement()->Velocity += FVector(GetActorForwardVector() * DashModifier * GetWorld()->DeltaTimeSeconds);
						}
					}
				}
			}
		}
	}

}

void AMyPlayer::EndCrouch()
{
	UnCrouch();
}

void AMyPlayer::BeginJump()
{
	if (bCanWallRun)
	{
		FLatentActionInfo ActionInfo;
		ActionInfo.CallbackTarget = this;

		SetActorLocation(GetActorLocation() + GetActorRightVector() * 65.0f);
		UKismetSystemLibrary::MoveComponentTo(RootComponent, startPoint_.GetLocation(), startPoint_.Rotator(), false, false, 0.1f, true, EMoveComponentAction::Move, ActionInfo);
		PlayAnimMontage(WallRunMontage, 1.2f);
		UKismetSystemLibrary::MoveComponentTo(RootComponent, target_, startPoint_.Rotator(), true, false, 0.8f, true, EMoveComponentAction::Move, ActionInfo);
	}
	else {
		 Jump();
	}
}

void AMyPlayer::EndJump()
{
	StopJumping();
}


// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Crouch", this, &AMyPlayer::BeginCrouch);
	PlayerInputComponent->BindAxis("Dash", this, &AMyPlayer::Dash);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPlayer::EndJump);


}

float AMyPlayer::GetHealth()
{
	return HealthPercentage;
}

float AMyPlayer::GetStamina()
{
	return StaminaPercentage;
}

void AMyPlayer::SetDamageState()
{
	bCanBeDamaged = true;
}

void AMyPlayer::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AMyPlayer::SetDamageState, 2.0f, false);
}

void AMyPlayer::SetStaminaValue()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = PreviousStamina + StaminaValue * StaminaCurve->GetFloatValue(TimelineValue);
	StaminaPercentage = CurveFloatValue;
	StaminaPercentage = FMath::Clamp(StaminaPercentage, 0.0f, 1.0f);
}

void AMyPlayer::StopWallRunAnim()
{
	StopAnimMontage(WallRunMontage);
}


float AMyPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	bCanBeDamaged = false;
	UpdateHealth(-DamageAmount);
	DamageTimer();

	OnHandleAnyDamage.Broadcast(this, DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void AMyPlayer::SetRunnableWall_Implementation(FVector target, FTransform startPoint, bool enable)
{
	bCanWallRun = enable;
	target_ = target;
	startPoint_ = startPoint;
}



void AMyPlayer::UpdateHealth(float HealthChange)
{
	Health += HealthChange;
	Health = FMath::Clamp(Health, 0.0f, FullHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / FullHealth;
}

void AMyPlayer::UpdateStamina(float StaminaChange)
{
	Stamina += StaminaChange;
	Stamina = FMath::Clamp(Stamina, 0.0f, FullStamina);
	PreviousStamina = StaminaPercentage;
	StaminaPercentage = Stamina / FullStamina;
	if (Stamina <= 1.0f)
	{
		bCanUseStamina = false;
	}
}

void AMyPlayer::RegenerateStamina() {
	if (bHasLostStamina) {
		UpdateStamina(0.2f);
		if (Stamina >= FullStamina) {
			bCanUseStamina = true;
			bHasLostStamina = false;
		}
	}

	OnRegenerateStamina.Broadcast();
}
