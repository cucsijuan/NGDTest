// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestPlayerState.h"
#include "ConstructorHelpers.h"
#include "NGDTestUserWidget.h"

ANGDTestPlayerState::ANGDTestPlayerState()
{
}

void ANGDTestPlayerState::DoScore_Implementation(int SetScore)
{
	Score += SetScore;
	AddExplodedCube();
	MulticastScore(Score);
}

bool ANGDTestPlayerState::DoScore_Validate(int Score)
{
	return true;
}

void ANGDTestPlayerState::MulticastScore_Implementation(int SetScore)
{
	Score = SetScore;
}

void ANGDTestPlayerState::AddExplodedCube()
{
	ExplodedCubes++;
	UE_LOG(LogTemp, Warning, TEXT("Cubes: %d"), ExplodedCubes);
}

int ANGDTestPlayerState::PopExplodedCubes()
{
	int temp = ExplodedCubes;
	ExplodedCubes = 0;
	return temp;
}




