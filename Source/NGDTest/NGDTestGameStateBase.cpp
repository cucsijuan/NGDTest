// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestGameStateBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NGDTestPlayerState.h"
#include "MagicCube.h"


void ANGDTestGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	SetSpawner();
	if (Spawner != NULL) SpawnCube();

}
void ANGDTestGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto& Player : PlayerArray)
	{
		if (Cast<ANGDTestPlayerState>(Player)->GetExplodedCubes() >= MAX_CUBES);
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
			Cube->AssignCubeColor(FMath::RandRange(0, 2));
			
			int Size = 150;
			for (int i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(FMath::RandRange(0, 2));
				Size += 150;
			}

			Size = -150;
			for (int i = 0; i < Step; i++)
			{
				AMagicCube * Cube = World->SpawnActor<AMagicCube>(MagicCubeClass, Spawner->GetActorLocation() + FVector(0, Size, Height), Spawner->GetActorRotation(), ActorSpawnParams);
				Cube->AssignCubeColor(FMath::RandRange(0, 2));
				Size -= 150;
			}
			Height += 150;
		}
	}
}

