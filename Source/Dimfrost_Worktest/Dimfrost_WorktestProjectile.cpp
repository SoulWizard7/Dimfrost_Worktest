// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dimfrost_WorktestProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ADimfrost_WorktestProjectile::ADimfrost_WorktestProjectile() 
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADimfrost_WorktestProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;	

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
}

void ADimfrost_WorktestProjectile::Tick(float DeltaSeconds)
{		
	if(isActive)
	{
		if(FVector::Dist2D(SpawnPos, GetActorLocation()) > MaxDistance)
		{			
			ProjectilePool->ReturnProjectile(this);
		}

		timeToKill -= DeltaSeconds;
		
		if(timeToKill < 0.f)
		{
			ProjectilePool->ReturnProjectile(this);
		}
	}
}

void ADimfrost_WorktestProjectile::EnableProjectile(float TimeToKill)
{	
	GetProjectileMovement()->SetUpdatedComponent(GetRootComponent()); //Resets the projectile movement component
	timeToKill = TimeToKill;
	isActive = true;
	SetActorHiddenInGame(false);	
}

void ADimfrost_WorktestProjectile::DisableProjectile()
{
	SetActorLocation(FVector::ZeroVector);
	ProjectileMovement->Velocity = FVector::Zero();
	SetActorHiddenInGame(true);
	isActive = false;
}

void ADimfrost_WorktestProjectile::ReturnToProjPool()
{
	ProjectilePool->ReturnProjectile(this);
}

void ADimfrost_WorktestProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		ProjectilePool->ReturnProjectile(this);
	}
}
