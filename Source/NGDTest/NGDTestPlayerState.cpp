// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestPlayerState.h"
#include "ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "NGDTestUserWidget.h"

ANGDTestPlayerState::ANGDTestPlayerState()
{
}

void ANGDTestPlayerState::DoScore_Implementation(int32 SetScore)
{

	PlayerScore += SetScore;
	MulticastScore(PlayerScore);
}

bool ANGDTestPlayerState::DoScore_Validate(int32 SetScore)
{
	return true;
}

void ANGDTestPlayerState::MulticastScore_Implementation(int32 SetScore)
{
	PlayerScore = SetScore;
}



