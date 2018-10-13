// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NGDTestProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "MagicCube.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ANGDTestProjectile::ANGDTestProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ANGDTestProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ANGDTestProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//If we are colliding with a MagicCube then begin cube's destroying process
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor->IsA(AMagicCube::StaticClass())))
	{
		if (HitComp->GetOwnerRole() == ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("Collided: %s"), *OtherActor->StaticClass()->GetFName().ToString());
			Cast<AMagicCube>(OtherActor)->Explode(Cast<APlayerController>(GetInstigator()->GetController())->PlayerState, 1);
		}
		else 
		{
			Cast<APlayerController>(GetInstigator()->GetController());
		}
		Destroy();
	}
}

bool ANGDTestProjectile::OnHit_Validate(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return true;
}