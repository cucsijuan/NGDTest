// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NGDTestGameMode.generated.h"

UCLASS(minimalapi)
class ANGDTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static const int32 MAX_CUBES = 31;

	ANGDTestGameMode();

	/* Called when a Projectile wants to destroy another cube */
	void CubeFound(class AMagicCube * Cube, class APlayerState * Player);
	
	/* Called when a cube wants to destroy another cube */
	void CubeFound(class AMagicCube * Cube, class APlayerState * Player, int ChainPosition, TArray<AMagicCube *> ExplodedArray);
	
	/* Gets Fibonacci number of a given index using a loop*/
	int ChainPositionToFibonacciRec(int ChainPosition);

	/* Sort and multicasts the GameState's PlayerStatesByScore array */
	void SortPlayerStatesByScore();

	

protected:
	/* Used to spawn cubes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TSubclassOf<class AMagicCube> MagicCubeClass;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController * NewPlayer) override;

private:
	/* used to set the Name of the players */
	UPROPERTY()
	int32 PlayerNum = 1;

	/* Holds the total number of cubes destroyed in the current game */
	UPROPERTY(Transient)
	int32 CurrentCubes = 0;

	/* Spawner reference */
	UPROPERTY()
	AActor * Spawner;

	/* Array of Cubes that has to be spawned To the right and left of the spawned center Cube
	in everi step of the pyramid */
	UPROPERTY()
	TArray<int32> PyramidSteps = { 3,3,2,2,1,1,0 };

	/* Finds the spawner asset and stores a reference  */
	void SetSpawner();

	/* Spawn and paints the cube Pyramid  */
	void SpawnCube();

	/* checks if the game should end, if true tells to the GameState to do so */
	void ShouldGameEnd(int32 CurrentCubes);

	/* Look for matching cubes to destroy, add score to player and destroy the given cube */
	void DestroyCube(AMagicCube * CubeToDestroy, APlayerState * InstigatorState, int32 ChainPosition = 1, const TArray<AMagicCube *>& ExplodedCubes = {});

	/* Find neighbouring cubes and check if they are the same color of the given cube */
	TArray<AMagicCube *> FindNearbyCubes(AMagicCube * Cube);
	
	/* Sort Function based on Recursive Insertion sort algorithm, 
	it sorts the given array of ANGDTestPlayerState from Higher to lower score*/
	void SortInsertion(TArray<class ANGDTestPlayerState *> & Array, int32 n);
};



