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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	TArray<class ANGDTestPlayerState *> SortedPlayers;

	ANGDTestGameStateBase();

	/* Ends the Game*/
	void EndGame();

	/* Returns the IsGameEnded value */
	UFUNCTION(BlueprintCallable)
	bool IsGameOver() const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSortedPlayers(const TArray<class ANGDTestPlayerState *> & PlayersArray);

protected:
	/* Used by other objects to check if the game has ended */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Game State")
	bool IsGameEnded = false;
	
};
