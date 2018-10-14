// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NGDTestGameMode.h"
#include "NGDTestHUD.h"
#include "NGDTestCharacter.h"
#include "NGDTestPlayerState.h"
#include "NGDTestGameStateBase.h"
#include "NGDTestPlayerController.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
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

void ANGDTestGameMode::BeginPlay()
{
	SetSpawner();
	if (Spawner != NULL) SpawnCube();
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

void ANGDTestGameMode::SetSpawner()
{
	TArray<AActor *> SpawnerArray;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Spawner")), SpawnerArray);

	if (SpawnerArray.Num() != 0) Spawner = SpawnerArray[0];
}

void ANGDTestGameMode::SpawnCube()
{
	UWorld* const World = GetWorld();
	MagicCubeClass = AMagicCube::StaticClass();

	TArray<int32> Colorlist = { 1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3 };

	Colorlist.Sort([this](const int32 Item1, const int32 Item2) {
		return FMath::FRand() < 0.5f;
	});

	if (World != NULL && MagicCubeClass != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		int Height = 0;
		// spawn cubes
		for (auto& Step : PyramidSteps)
		{

			AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, 0, Height), Spawner->GetActorRotation(), ActorSpawnParams);
			Cube->AssignCubeColor(FMath::RandRange(1, 3));

			int Size = 150;
			for (int32 i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(Colorlist.Pop());
				Size += 150;
			}

			Size = -150;
			for (int32 i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(Colorlist.Pop());
				Size -= 150;
			}
			Height += 150;
		}
	}

}
