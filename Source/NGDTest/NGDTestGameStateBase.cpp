// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestGameStateBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "NGDTestPlayerState.h"
#include "NGDTestUserWidget.h"
#include "NGDTestPlayerController.h"
#include "MagicCube.h"


TMap<int32,int32> ANGDTestGameStateBase::GetScoreSortedPlayers()
{
	TMap<int32, int32> TempMap;
	int32 i=1;
	for (auto& Player : PlayerArray)
	{
		TempMap.Add(i, Player->Score);
		i++;
	}

	TempMap.ValueSort([](int32 A, int32 B) {
		return A > B; 
	});

	return TempMap;
}

ANGDTestGameStateBase::ANGDTestGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANGDTestGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetSpawner();
		if (Spawner != NULL) SpawnCube();
	}
	

}
void ANGDTestGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCurrentExplodedCubes();
	if (!IsGameOver) EndGame();
}

void ANGDTestGameStateBase::GetCurrentExplodedCubes()
{
	for (auto& Player : PlayerArray)
	{
		CurrentCubes += Cast<ANGDTestPlayerState>(Player)->PopExplodedCubes();
	}
}

void ANGDTestGameStateBase::EndGame()
{
	if (CurrentCubes >= MAX_CUBES)
	{
		IsGameOver = true;
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
			GameOverWidget->GameOver();
		}
	}
}

void ANGDTestGameStateBase::SetSpawner()
{
	TArray<AActor *> SpawnerArray;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Spawner")), SpawnerArray);

	if (SpawnerArray.Num() != 0) Spawner = SpawnerArray[0];
}

void ANGDTestGameStateBase::SpawnCube()
{
	UWorld* const World = GetWorld();
	MagicCubeClass = AMagicCube::StaticClass();

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
			for (int i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(FMath::RandRange(1, 3));
				Size += 150;
			}

			Size = -150;
			for (int i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(FMath::RandRange(1, 3));
				Size -= 150;
			}
			Height += 150;
		}
	}
}

