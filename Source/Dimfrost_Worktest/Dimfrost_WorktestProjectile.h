// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DW_ProjectilePool.h"
#include "GameFramework/Actor.h"
#include "Dimfrost_WorktestProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class ADimfrost_WorktestProjectile : public AActor
{
	GENERATED_BODY()	

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ADimfrost_WorktestProjectile();
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY()
	ADW_ProjectilePool* ProjectilePool;

	UPROPERTY()
	FVector SpawnPos = FVector::Zero();

	UPROPERTY()
	float MaxDistance = 200.f;	

	UFUNCTION()
	void EnableProjectile(float TimeToKill);

	UFUNCTION()
	void DisableProjectile();

	UFUNCTION()
	void ReturnToProjPool();

private:
	
	UPROPERTY()
	bool isActive = false;

	UPROPERTY()
	float timeToKill = 3.f;

public:

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};



