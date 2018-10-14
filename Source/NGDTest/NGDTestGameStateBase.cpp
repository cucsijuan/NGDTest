// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestGameStateBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "NGDTestPlayerState.h"
#include "NGDTestGameMode.h"
#include "NGDTestPlayerController.h"
#include "MagicCube.h"




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
	//Check if the game should be ending
	if (GetWorld()->GetAuthGameMode() && !IsGameOver)
		IsGameOver = Cast<ANGDTestGameMode>(GetWorld()->GetAuthGameMode())->EndGame(CurrentCubes);
}

void ANGDTestGameStateBase::GetCurrentExplodedCubes()
{
	for (auto& Player : PlayerArray)
	{
		CurrentCubes += Cast<ANGDTestPlayerState>(Player)->PopExplodedCubes();
	}
}

TMap<int32, int32> ANGDTestGameStateBase::GetScoreSortedPlayers()
{
	TMap<int32, int32> TempMap;
	int32 i = 1;
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
			Cube->AssignCubeColor(FMath::RandRange(1,3));
			
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

void ANGDTestGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ANGDTestGameStateBase, IsGameOver);
}

