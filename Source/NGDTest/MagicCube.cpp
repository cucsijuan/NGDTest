// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicCube.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "NGDTestPlayerState.h"

AMagicCube::AMagicCube()
{
	bReplicates = true;
	bReplicateMovement = true;
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

	//Setup Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<name>"));
	MeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeAsset(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));

	if (CubeAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		MeshComponent->SetWorldScale3D(FVector(1.5f));
	}

	//Setup Mesh material
	static ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset(TEXT("/Game/FirstPerson/Meshes/BaseMaterial.BaseMaterial"));
	if (MaterialAsset.Succeeded())
	{
		Material = MaterialAsset.Object;
		
	}

}

void AMagicCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(AMagicCube, Material);
	DOREPLIFETIME(AMagicCube, MeshComponent);
}

void AMagicCube::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldFall() && !Falling)
	{
		FVector Actorsize;
		FVector Origin;
		//start falling
		Falling = true;
		GetActorBounds(false, Origin, Actorsize);
		NewLocation = GetActorLocation() + FVector(0, 0, -Actorsize.Z * 2);


		DoFalling(DeltaTime);
	}
	else if (Falling)
	{
		DoFalling(DeltaTime);
	}

}

void AMagicCube::DoFalling(float DeltaTime)
{
	FVector Location = GetActorLocation();
	FVector Direction = -GetActorUpVector().GetSafeNormal();
	Location += Direction * DeltaTime * Speed;
	SetActorLocation(Location);
	if (NewLocation.Z >= GetActorLocation().Z)
	{
		SetActorLocation(NewLocation);
		Falling = false;
	}
}

bool AMagicCube::ShouldFall()
{
	FVector UpOffset = FVector(0.f, 0.f, 40.f);
	FVector Start = GetActorLocation();
	FVector DownVector = -GetActorUpVector();
	FVector DownEnd = ((DownVector * 90.f) + Start);
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, this);
	FCollisionResponseParams  CollisionRespParams;
	FHitResult OutHit;

	DrawDebugLine(GetWorld(), Start - UpOffset, DownEnd, FColor::Purple, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - UpOffset, DownEnd, ECC_Visibility, CollisionParams, CollisionRespParams))
	{
		return false;
	}
	else {
		return true;
	}
}

void AMagicCube::AssignCubeColor(int ColorNum)
{
	DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DynMaterial->SetVectorParameterValue("DiffuseColor", Colors[ColorNum]);
	DynMaterial->SetVectorParameterValue("EmissiveColor", Colors[ColorNum]);
	DynMaterial->SetScalarParameterValue("EmissiveAmount", 2.f);
	SetColorName(ColorNum);
	
	MeshComponent->SetMaterial(0, DynMaterial);
	
}

void AMagicCube::Explode(APlayerState * InstigatorState,int ChainPosition, TArray<AMagicCube *> ExplodedCubes)
{
	if (Exploding) return;

	Exploding = true;
	
	//increase player Score
	UE_LOG(LogTemp, Warning, TEXT("Chain: %d"), ChainPosition);
	Cast<ANGDTestPlayerState>(InstigatorState)->DoScore(ChainPosition);

	//Find and save cubes with the same color
	TArray<AMagicCube *> FoundCubes = FindNearbyCubes();
	
	for (auto& Cube : FoundCubes)
	{
		//check if the cube already exploded
		if (!ExplodedCubes.Contains(Cube))
		{
			//Store the found cubes along with the ones who exploded
			//so the next cube knows who already exploded or is set to explode
			TArray<AMagicCube *> TempCubes;
			TempCubes.Append(ExplodedCubes);
			TempCubes.Append(FoundCubes);
			Cube->Explode(InstigatorState, ChainPosition + 1, TempCubes);
		}
	}

	
	Destroy();
}

void AMagicCube::OnRep_Material(UMaterialInstanceDynamic * Mat)
{
	MeshComponent->SetMaterial(0, Mat);
}

TArray<AMagicCube *> AMagicCube::FindNearbyCubes()
{
	//we will store all the found cube in this array
	TArray<AMagicCube *> CubesToDestroy;
	
	//this offsets are used to draw the raytrace outside the owner to avoid self detection
	FVector RightOffset = FVector(0.f, 40.f, 0.f);
	FVector UpOffset = FVector(0.f, 0.f, 40.f);

	//LineTrace vector setup
	FVector Start = GetActorLocation();
	FVector RightVector = GetActorRightVector();
	FVector RightEnd = ((RightVector * 90.f) + Start);
	FVector LeftVector = -GetActorRightVector();
	FVector LeftEnd = ((LeftVector * 90.f) + Start);
	FVector UpVector = GetActorUpVector();
	FVector UpEnd = ((UpVector * 90.f) + Start);
	FVector DownVector = -GetActorUpVector();
	FVector DownEnd = ((DownVector * 90.f) + Start);
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, this);
	FCollisionResponseParams  CollisionRespParams;

	FHitResult OutHit;

	DrawDebugLine(GetWorld(), Start + RightOffset, RightEnd, FColor::Red, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start + RightOffset, RightEnd, ECC_Visibility, CollisionParams, CollisionRespParams) 
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	DrawDebugLine(GetWorld(), Start - RightOffset, LeftEnd, FColor::Green, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - RightOffset, LeftEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	DrawDebugLine(GetWorld(), Start + UpOffset, UpEnd, FColor::Blue, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start + UpOffset, UpEnd, ECC_Visibility, CollisionParams, CollisionRespParams) 
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	DrawDebugLine(GetWorld(), Start - UpOffset, DownEnd, FColor::Purple, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - UpOffset, DownEnd, ECC_Visibility, CollisionParams, CollisionRespParams)
		&& (OutHit.GetActor()->IsA(AMagicCube::StaticClass())))
	{
		if (IsSameColor(Cast<AMagicCube>(OutHit.GetActor()))) CubesToDestroy.Add(Cast<AMagicCube>(OutHit.GetActor()));
	}

	return CubesToDestroy;
}

bool AMagicCube::IsSameColor(AMagicCube * other) const
{
	return (other->GetColorName() == GetColorName());
}

FString AMagicCube::GetColorName() const
{
	return CurrentColorName;
}

void AMagicCube::SetColorName(int ColorID)
{
	switch (ColorID)
	{
	case 0:
		CurrentColorName = FString(TEXT("Red"));
		break;
	case 1:
		CurrentColorName = FString(TEXT("Blue"));
		break;
	case 2:
		CurrentColorName = FString(TEXT("Green"));
		break;
	default:
		break;
	}

}



