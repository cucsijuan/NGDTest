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

void ANGDTestGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ANGDTestGameStateBase, IsGameOver);
}

