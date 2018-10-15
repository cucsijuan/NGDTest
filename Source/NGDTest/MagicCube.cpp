// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicCube.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "NGDTestGameMode.h"
#include "NGDTestPlayerState.h"

AMagicCube::AMagicCube()
{
	bReplicates = true;
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

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start - UpOffset, DownEnd, ECC_Visibility, CollisionParams, CollisionRespParams))
	{
		return false;
	}
	else {
		return true;
	}
}

void AMagicCube::AssignCubeColor(int32 ColorNum)
{
	DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DynMaterial->SetVectorParameterValue("DiffuseColor", Colors[ColorNum]);
	DynMaterial->SetVectorParameterValue("EmissiveColor", Colors[ColorNum]);
	DynMaterial->SetScalarParameterValue("EmissiveAmount", 2.f);
	SetColorName(ColorNum);
	
	MeshComponent->SetMaterial(0, DynMaterial);
	
}

void AMagicCube::OnRep_DynMaterial()
{
	AssignCubeColor(CurrentColorName);
}

bool AMagicCube::IsSameColor(AMagicCube * other) const
{
	return (other->GetColorName() == GetColorName());
}

bool AMagicCube::IsExploding()
{
	return Exploding;
}

void AMagicCube::SetExploding(bool status)
{
	Exploding = status;
}

int AMagicCube::GetColorName() const
{
	return CurrentColorName;
}

void AMagicCube::SetColorName(int32 ColorID)
{
		CurrentColorName = ColorID;
}

void AMagicCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagicCube, CurrentColorName);
}




