// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MagicCube.generated.h"

/**
 * 
 */
UCLASS()
class NGDTEST_API AMagicCube : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Cube Falling Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Speed = 250.f;
	
	AMagicCube();

	/* Look for matching cubes to explode, add score to player and destroy itself */
	UFUNCTION(NetMulticast,Reliable, WithValidation)
	void Explode(class APlayerState * InstigatorState, int ChainPosition = 0, const TArray<AMagicCube *>& ExplodedCubes = {});
	
	/* sets color of the cube ColorNum must be a valid index of TArray <FColor> Colors  */
	void AssignCubeColor(int ColorNum);
	
	/* Returns Color Index */
	int GetColorName() const;
	
	/* Sets Color Index */
	void SetColorName(int ColorID);
	
	/* Checks if other cube has the same color of this one */
	bool IsSameColor(AMagicCube * other) const;

private:
	/* Cube's Mesh */
	UPROPERTY()
	UStaticMeshComponent * MeshComponent;

	/* Cube's Material */
	UPROPERTY()
	class UMaterial * Material;

	/* Cube's Material Instance */
	UPROPERTY()
	class UMaterialInstanceDynamic * DynMaterial;

	/* Color Array, its used to set the color of the cube based on a index */
	UPROPERTY()
	TArray <FColor> Colors = { FColor::Black, FColor::Red,FColor::Blue,FColor::Green};

	/* The curren color index of the cube */
	UPROPERTY(ReplicatedUsing = OnRep_DynMaterial)
	int CurrentColorName;

	/* used to check if the cube can explode or its already exploding */
	UPROPERTY()
	bool Exploding = false;

	/* used to check if the cube can fall or its already falling */
	UPROPERTY()
	bool Falling = false;

	/* Contains the location wich the cube has to move */
	UPROPERTY()
	FVector NewLocation;

	/* Find neighbour cubes and checks if they are the same color of this cube */
	UPROPERTY()
	TArray<AMagicCube *> FindNearbyCubes();

	/* used to check if the cube has to start falling */
	UPROPERTY()
	bool ShouldFall();

	/* Replicates DynMaterial property*/
	UFUNCTION()
	void OnRep_DynMaterial();
	
	/* Handles the cube falling */
	void DoFalling(float DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
};
