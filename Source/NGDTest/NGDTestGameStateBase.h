// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NGDTestGameStateBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyDelegate);

UCLASS()
class NGDTEST_API ANGDTestGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	const int MAX_CUBES = 28;

	UPROPERTY()
	TSubclassOf<class AMagicCube> MagicCubeClass;
	UPROPERTY()
	AActor * Spawner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UNGDTestUserWidget * GameOverWidget;
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FDestroyDelegate DestroyDelegate;
	
	ANGDTestGameStateBase();

	void SetSpawner();
	void SpawnCube();

private:
	UPROPERTY()
	TArray<int> PyramidSteps = { 3,3,2,2,1,1,0 };
	UPROPERTY()
	bool IsGameOver = false;

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	void EndGame();
};
