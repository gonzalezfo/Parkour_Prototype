// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/PlayerHUDWidget.h"
#include "../Public/MyPlayer.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	myPlayerReference = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(!myPlayerReference->OnHandleAnyDamage.Contains(this, "UpdateHealthBar"))
	{
		myPlayerReference->OnHandleAnyDamage.AddDynamic(this, &UPlayerHUDWidget::UpdateHealthBar);
	}

	if (!myPlayerReference->OnRegenerateStamina.Contains(this, "UpdateStaminaBar"))
	{
		myPlayerReference->OnRegenerateStamina.AddDynamic(this, &UPlayerHUDWidget::UpdateStaminaBar);
	}
}

void UPlayerHUDWidget::UpdateHealthBar(AMyPlayer * myPlayer, float Health, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	HealthBar->SetPercent(myPlayerReference->GetHealth());
}

void UPlayerHUDWidget::UpdateStaminaBar()
{
	StaminaBar->SetPercent(myPlayerReference->GetStamina());
}
