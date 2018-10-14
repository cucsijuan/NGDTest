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
	
	/* Gets Fibonacci number of a given index using a formula*/
	int ChainPositionToFibonacci(int ChainPosition);
	
	/* Gets Fibonacci number of a given index using a loop*/
	int ChainPositionToFibonacciRec(int ChainPosition);

	bool EndGame(int32 CurrentCubes);
protected:
	virtual void BeginPlay() override;
private:
	/* Used to spawn cubes */
	UPROPERTY()
	TSubclassOf<class AMagicCube> MagicCubeClass;

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
};



