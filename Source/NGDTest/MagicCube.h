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
	AMagicCube();
	// Cube Falling Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Speed = 250.f;

	/* Look for matching cubes to explode, add score to player and destroy itself */
	//UFUNCTION(NetMulticast,Reliable, WithValidation)
	void Explode(class APlayerState * InstigatorState, int32 ChainPosition = 1, const TArray<AMagicCube *>& ExplodedCubes = {});
	
	/* sets color of the cube ColorNum must be a valid index of TArray <FColor> Colors  */
	void AssignCubeColor(int32 ColorNum);
	
	/* Returns Color Index */
	int GetColorName() const;
	
	/* Sets Color Index */
	void SetColorName(int32 ColorID);
	
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
	int32 CurrentColorName;

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
	TArray<AMagicCube *> FindNearbyCubes();

	/* Replicates DynMaterial property*/
	UFUNCTION()
	void OnRep_DynMaterial();
	
	/* Handles the cube falling */
	void DoFalling(float DeltaTime);

	/* used to check if the cube has to start falling */
	bool ShouldFall();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	
	
};
