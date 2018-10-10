// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NGDTestGameMode.h"
#include "NGDTestHUD.h"
#include "NGDTestCharacter.h"
#include "NGDTestPlayerState.h"
#include "NGDTestGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

ANGDTestGameMode::ANGDTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Set Custom Classes
	HUDClass = ANGDTestHUD::StaticClass();
	PlayerStateClass = ANGDTestPlayerState::StaticClass();
	GameStateClass = ANGDTestGameStateBase::StaticClass();
}
