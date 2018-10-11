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
private:
	UPROPERTY()
	TArray<int> PyramidSteps = { 3,3,2,2,1,1,0 };
public:
	UPROPERTY()
	static const int MAX_CUBES = 28;
	UPROPERTY()
	TSubclassOf<class AMagicCube> MagicCubeClass;
	UPROPERTY()
	AActor * Spawner;

	void SetSpawner();
	void SpawnCube();

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
};
