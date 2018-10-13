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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> AssetScoreWidget;

	class UUserWidget * ScoreWidget;

	ANGDTestPlayerController();
	

protected:
	virtual void BeginPlay() override;
	
	
};
