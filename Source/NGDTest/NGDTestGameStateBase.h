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
	const int32 MAX_CUBES = 31;

	UPROPERTY(Transient)
		int32 CurrentCubes = 0;

	UPROPERTY()
	TSubclassOf<class AMagicCube> MagicCubeClass;

	UPROPERTY()
	AActor * Spawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UNGDTestUserWidget * GameOverWidget;

	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> GetScoreSortedPlayers();
	
	
	ANGDTestGameStateBase();

	void SetSpawner();

	void SpawnCube();

private:
	UPROPERTY()
	TArray<int> PyramidSteps = { 3,3,2,2,1,1,0 };
	
	UPROPERTY()
	TMap<int32, int32> ScoreSortedPlayers;

	UPROPERTY()
	bool IsGameOver = false;

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	void GetCurrentExplodedCubes();
	void EndGame();
};
