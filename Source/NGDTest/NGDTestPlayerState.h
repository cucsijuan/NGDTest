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
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerScore = 0;

	/* Sets the player score serverside */
	UFUNCTION(Server, Reliable, WithValidation)
	void DoScore(int32 SetScore);

	UFUNCTION(NetMulticast, Reliable)
	void  MulticastScore(int32 SetScore);

	

};
