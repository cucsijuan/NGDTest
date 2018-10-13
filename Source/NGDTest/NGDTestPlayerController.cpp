// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestPlayerController.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "NGDTestGameStateBase.h"
#include "NGDTestUserWidget.h"

ANGDTestPlayerController::ANGDTestPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UClass>ScoreWidgetFinder(TEXT("WidgetBlueprint'/Game/FirstPersonCPP/Blueprints/BP_ScoreWidget.BP_ScoreWidget_C'"));
	if (ScoreWidgetFinder.Succeeded())
	AssetScoreWidget = ScoreWidgetFinder.Object;
	
}



void ANGDTestPlayerController::BeginPlay()
{
	if (IsLocalPlayerController())
	{
		if (AssetScoreWidget)
			ScoreWidget = CreateWidget<UUserWidget>(this, AssetScoreWidget);
	
		if (ScoreWidget) ScoreWidget->AddToViewport();
	}
}
