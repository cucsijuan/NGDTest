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
	UPROPERTY(Replicated)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(Replicated)
	class UMaterial * Material;
	UPROPERTY(ReplicatedUsing = OnRep_Material)
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
	
	UFUNCTION()
	void OnRep_Material(UMaterialInstanceDynamic * Mat);
	TArray<AMagicCube *> FindNearbyCubes();
	bool ShouldFall();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Speed = 250.f;
	
	AMagicCube();
	UFUNCTION()
	void Explode(class APlayerState * InstigatorState, int ChainPosition = 0, TArray<AMagicCube *> ExplodedCubes = {});
	UFUNCTION()
	FString GetColorName() const;
	UFUNCTION()
	void SetColorName(int ColorID);
	UFUNCTION()
	bool IsSameColor(AMagicCube * other) const;
	UFUNCTION()
	void AssignCubeColor(int ColorNum);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DoFalling(float DeltaTime);
	
};
