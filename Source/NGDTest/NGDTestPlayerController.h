// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NGDTestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NGDTEST_API ANGDTestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UFUNCTION(Server, unreliable, WithValidation)
	void Server_ExplodeCube(class AMagicCube * CubeToExplode);
	
	
};
