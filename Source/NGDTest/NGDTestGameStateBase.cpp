// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestGameStateBase.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "NGDTestPlayerState.h"


ANGDTestGameStateBase::ANGDTestGameStateBase()
{
}

void ANGDTestGameStateBase::EndGame()
{
	if(HasAuthority())
		IsGameEnded = true;
}

bool ANGDTestGameStateBase::IsGameOver() const
{
	return IsGameEnded;
}

void ANGDTestGameStateBase::MulticastSortedPlayers_Implementation(const TArray<ANGDTestPlayerState *> & PlayersArray)
{
	SortedPlayers = PlayersArray;
}

void ANGDTestGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANGDTestGameStateBase, IsGameEnded); 
	
}


