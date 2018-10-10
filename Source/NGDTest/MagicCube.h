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
	
private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;
	UPROPERTY()
	class UMaterial * Material;
	UPROPERTY()
	class UMaterialInstanceDynamic * DynMaterial;
	UPROPERTY()
	TArray <FColor> Colors = { FColor::Red,FColor::Blue,FColor::Green};
	UPROPERTY()
	FString CurrentColorName;
	UPROPERTY()
	bool Exploding = false;
	UPROPERTY()
	bool Falling = false;
	UPROPERTY()
	FVector NewLocation;

	TArray<AMagicCube *> FindNearbyCubes();
	bool ShouldFall();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Speed = 250.f;
	
	AMagicCube();
	void Explode(class APlayerState * InstigatorState, int ChainPosition = 0, TArray<AMagicCube *> ExplodedCubes = {});
	FString GetColorName() const;
	void SetColorName(int ColorID);
	bool IsSameColor(AMagicCube * other) const;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DoFalling(float DeltaTime);
	void AssignCubeColor();
};
