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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Speed = 250.f;
	
	AMagicCube();
	UFUNCTION(NetMulticast,Reliable, WithValidation)
	void Explode(class APlayerState * InstigatorState, int ChainPosition = 0, const TArray<AMagicCube *>& ExplodedCubes = {});
	void AssignCubeColor(int ColorNum);
	int GetColorName() const;
	void SetColorName(int ColorID);
	bool IsSameColor(AMagicCube * other) const;

private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;
	UPROPERTY()
	class UMaterial * Material;
	UPROPERTY()
	class UMaterialInstanceDynamic * DynMaterial;
	UPROPERTY()
	TArray <FColor> Colors = { FColor::Black, FColor::Red,FColor::Blue,FColor::Green};
	UPROPERTY(ReplicatedUsing = OnRep_DynMaterial)
	int CurrentColorName;
	UPROPERTY()
	bool Exploding = false;
	UPROPERTY()
	bool Falling = false;
	UPROPERTY()
	FVector NewLocation;

	UFUNCTION()
	void OnRep_DynMaterial();
	
	TArray<AMagicCube *> FindNearbyCubes();
	bool ShouldFall();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DoFalling(float DeltaTime);
	
};
