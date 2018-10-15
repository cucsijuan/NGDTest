// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NGDTestGameMode.h"
#include "NGDTestHUD.h"
#include "NGDTestCharacter.h"
#include "NGDTestPlayerState.h"
#include "NGDTestGameStateBase.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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
	MagicCubeClass = AMagicCube::StaticClass();
	
}

void ANGDTestGameMode::BeginPlay()
{
	SetSpawner();
	if (Spawner != NULL) SpawnCube();
}

void ANGDTestGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NewPlayer->PlayerState->SetPlayerName("Player " + FString::FromInt(PlayerNum));
	PlayerNum++;
}

void ANGDTestGameMode::CubeFound(AMagicCube * Cube, APlayerState * Player)
{
	DestroyCube(Cube, Player);
	Cast<ANGDTestPlayerState>(Player)->DoScore(ChainPositionToFibonacciRec(1));
	CurrentCubes++;
	ShouldGameEnd(CurrentCubes);
}

void ANGDTestGameMode::CubeFound(AMagicCube * Cube, APlayerState * Player, int ChainPosition, TArray<AMagicCube *> ExplodedArray)
{
	DestroyCube(Cube, Player, ChainPosition, ExplodedArray);
	Cast<ANGDTestPlayerState>(Player)->DoScore(ChainPositionToFibonacciRec(ChainPosition));
	CurrentCubes++;
	ShouldGameEnd(CurrentCubes);
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

void ANGDTestGameMode::SortPlayerStatesByScore()
{
	ANGDTestGameStateBase * TempGameState = GetGameState<ANGDTestGameStateBase>();
	TArray<ANGDTestPlayerState *> SortedPlayerArray;

	for (auto& Player : TempGameState->PlayerArray)
	{
		SortedPlayerArray.Add(Cast<ANGDTestPlayerState>(Player));
	};
	
	SortInsertion(SortedPlayerArray, SortedPlayerArray.Num() - 1);

	TempGameState->MulticastSortedPlayers(SortedPlayerArray);

}

void ANGDTestGameMode::SortInsertion(TArray<ANGDTestPlayerState*>& Array, int32 Index)
{
	if (Index > 0)
	{
		SortInsertion(Array, Index - 1);

		ANGDTestPlayerState * x = Array[Index];
		int32 j = Index - 1;

		while (j >= 0 && Array[j]->PlayerScore < x->PlayerScore)
		{
			Array[j + 1] = Array[j];
			j = j - 1;
		}
		Array[j + 1] = x;
	}
		
}

void ANGDTestGameMode::ShouldGameEnd(int32 CurrentCubes)
{
	if (CurrentCubes >= MAX_CUBES)
	{
		SortPlayerStatesByScore();
		GetGameState<ANGDTestGameStateBase>()->EndGame();
	}
		
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
	
	//an array with preloaded colors
	TArray<int32> Colorlist = { 1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3 };
	
	//shuffles the color array
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

void ANGDTestGameMode::DestroyCube(AMagicCube * CubeToDestroy, APlayerState * InstigatorState, int32 ChainPosition, const  TArray<AMagicCube *>& ExplodedCubes)
{
	if (CubeToDestroy->IsExploding()) return;

	CubeToDestroy->SetExploding(true);

	//Find and save cubes with the same color
	TArray<AMagicCube *> FoundCubes = FindNearbyCubes(CubeToDestroy);

	for (auto& Cube : FoundCubes)
	{
		//check if the cube already exploded
		if (!ExplodedCubes.Contains(Cube))
		{
			//Store the found cubes along with the ones who exploded
			//so the next cube knows who already exploded or if its set to explode
			TArray<AMagicCube *> TempCubes;
			TempCubes.Append(ExplodedCubes);
			TempCubes.Append(FoundCubes);
			TempCubes.Add(CubeToDestroy);

			//Tell to the game mode that we want to destroy the found cube
			
			CubeFound(Cube,InstigatorState, ChainPosition + 1, TempCubes);
		}
	}
	
	CubeToDestroy->Destroy();
}

TArray<AMagicCube *> ANGDTestGameMode::FindNearbyCubes(AMagicCube * Cube)
{
	//we will store all the found cube in this array
	TArray<AMagicCube *> CubesToDestroy;

	//this offsets are used to draw the raytrace outside the owner to avoid self detection
	FVector RightOffset = FVector(0.f, 40.f, 0.f);
	FVector UpOffset = FVector(0.f, 0.f, 40.f);

	//LineTrace vector setup
	FVector Start = Cube->GetActorLocation();
	FVector RightVector = Cube->GetActorRightVector();
	FVector RightEnd = ((RightVector * 90.f) + Start);
	FVector LeftVector = -Cube->GetActorRightVector();
	FVector LeftEnd = ((LeftVector * 90.f) + Start);
	FVector UpVector = Cube->GetActorUpVector();
	FVector UpEnd = ((UpVector * 90.f) + Start);
	FVector DownVector = -Cube->GetActorUpVector();
	FVector DownEnd = ((DownVector * 90.f) + Start);
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, Cube);
	FCollisionResponseParams  CollisionRespParams;

	FHitResult OutHit;
	
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start + RightOffset, RightEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (Cube->IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - RightOffset, LeftEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (Cube->IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start + UpOffset, UpEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (Cube->IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - UpOffset, DownEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (Cube->IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	return CubesToDestroy;
}
