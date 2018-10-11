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
	void DoScore(int ChainPosition);
	void AddExplodedCube();
	int GetExplodedCubes();
private:
	UPROPERTY()
	int ExplodedCubes;
	int ChainPositionToFibonacci(int ChainPosition);
	int ChainPositionToFibonacciRec(int ChainPosition);
};
