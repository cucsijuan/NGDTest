// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NGDTestGameStateBase.generated.h"

/**
 * 
 */

UCLASS()
class NGDTEST_API ANGDTestGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	/* Used by other objects to check if the game has ended */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Game State")
	bool IsGameOver = false;

	/* This function sorts the plyers by their score, return a Map
	With the Player number and his Score */
	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> GetScoreSortedPlayers();

	ANGDTestGameStateBase();


private:

	/* Map with Playernames sorted by their score */
	UPROPERTY()
	TMap<int32, int32> ScoreSortedPlayers;

	/* Spawner reference */
	UPROPERTY()
	AActor * Spawner;

protected:
	/* Holds the total number of cubes destroyed in the current game */
	UPROPERTY(Transient)
	int32 CurrentCubes = 0;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/* Returns the CurrentCubes property  */
	void GetCurrentExplodedCubes();
};
