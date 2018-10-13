// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NGDTestPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class NGDTEST_API ANGDTestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
		
	ANGDTestPlayerState();

	void DoScore(int ChainPosition);

	void GameOver();

	/*  Add a Cube to ExplodedCubes property */
	void AddExplodedCube();

	/*Returns the ExplodedCubes property and resets it*/
	int PopExplodedCubes();

private:
	/* Stores the exploded cubes until GameState gets them */
	UPROPERTY()
	int ExplodedCubes=0;

	/* Gets Fibonacci based on a formula */
	int ChainPositionToFibonacci(int ChainPosition);

	/* Gets Fibonacci based on a loop */
	int ChainPositionToFibonacciRec(int ChainPosition);
};
