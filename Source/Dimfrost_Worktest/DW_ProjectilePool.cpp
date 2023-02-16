// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_ProjectilePool.h"

#include "Dimfrost_WorktestProjectile.h"


// Sets default values
ADW_ProjectilePool::ADW_ProjectilePool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

ADimfrost_WorktestProjectile* ADW_ProjectilePool::GetProjectile()
{
	if(ProjPool.Num() < 1)
	{		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int i = 0; i < 10; i++)
		{
			ADimfrost_WorktestProjectile* proj = GetWorld()->SpawnActor<ADimfrost_WorktestProjectile>(ProjectileClass, FVector::Zero(), FRotator::ZeroRotator, ActorSpawnParams);
			proj->ProjectilePool = this;
			proj->DisableProjectile();
			ProjPool.Push(proj);
		}
	}
	ADimfrost_WorktestProjectile* proj = ProjPool.Pop(true);
	return proj;
}

void ADW_ProjectilePool::ReturnProjectile(ADimfrost_WorktestProjectile* proj)
{
	proj->DisableProjectile();
	ProjPool.Push(proj);
}

// Called when the game starts or when spawned
void ADW_ProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 20.f, FColor::Red, FString::Printf(TEXT("ProjectileClass NULL on BP_ProjectilePool")) , true, FVector2D(1.f));
		return;
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < 20; i++)
	{
		ADimfrost_WorktestProjectile* proj = GetWorld()->SpawnActor<ADimfrost_WorktestProjectile>(ProjectileClass, FVector::Zero(), FRotator::ZeroRotator, ActorSpawnParams);
		proj->ProjectilePool = this;
		proj->DisableProjectile();
		ProjPool.Push(proj);
	}	
}

