// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestPlayerController.h"
#include "Engine/World.h"
#include "NGDTestGameStateBase.h"

void ANGDTestPlayerController::Server_ExplodeCube_Implementation(AMagicCube * CubeToExplode)
{
	//https://forums.unrealengine.com/development-discussion/c-gameplay-programming/32533-c-event-dispatchers-how-do-we-implement-them
	//hay que poner el dynamic en el game estate y luego llamarlo desde aca
}
bool ANGDTestPlayerController::Server_ExplodeCube_Validate(AMagicCube * CubeToExplode)
{
	return true;
}