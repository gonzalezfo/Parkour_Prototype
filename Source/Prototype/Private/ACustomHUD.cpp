// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ACustomHUD.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"



AACustomHUD::AACustomHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/UI/BP_PlayerHUDWidget"));
	HUDWidgetClass = HealthBarObj.Class;
}

void AACustomHUD::DrawHUD()
{
	Super::DrawHUD();

}

void AACustomHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

