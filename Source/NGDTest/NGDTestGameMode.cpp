// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NGDTestGameMode.h"
#include "NGDTestHUD.h"
#include "NGDTestCharacter.h"
#include "NGDTestPlayerState.h"
#include "NGDTestGameStateBase.h"
#include "NGDTestPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "MagicCube.h"

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

void ANGDTestGameMode::CubeFound(AMagicCube * Cube, APlayerState * Player)
{
	Cube->Explode(Player, 1);
	Cast<ANGDTestPlayerState>(Player)->DoScore(ChainPositionToFibonacciRec(1));
	UE_LOG(LogTemp, Warning, TEXT("ProjectileCall"));
}

void ANGDTestGameMode::CubeFound(AMagicCube * Cube, APlayerState * Player, int ChainPosition, TArray<AMagicCube *> ExplodedArray)
{
	Cube->Explode(Player, ChainPosition, ExplodedArray);
	Cast<ANGDTestPlayerState>(Player)->DoScore(ChainPositionToFibonacciRec(ChainPosition));
	UE_LOG(LogTemp, Warning, TEXT("CubeCall"));
}

int ANGDTestGameMode::ChainPositionToFibonacci(int ChainPosition)
{
	//We use Binet's Fibonacci Number Formula to get the value at a given index
	return (FMath::Pow((1 + FMath::Sqrt(5)), ChainPosition) - FMath::Pow((1 - FMath::Sqrt(5)), ChainPosition)) /
		(FMath::Pow(2, ChainPosition) * FMath::Sqrt(5));
}

int ANGDTestGameMode::ChainPositionToFibonacciRec(int ChainPosition)
{
	int a = 1, b = 1;
	for (int i = 3; i <= ChainPosition; i++) {
		int c = a + b;
		a = b;
		b = c;
	}
	return b;
}

bool ANGDTestGameMode::EndGame(int32 CurrentCubes)
{
	if (CurrentCubes >= MAX_CUBES)
	{
		return true;
	}
	return false;
}
