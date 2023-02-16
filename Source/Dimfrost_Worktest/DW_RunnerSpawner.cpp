// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_RunnerSpawner.h"


// Sets default values
ADW_RunnerSpawner::ADW_RunnerSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_RunnerSpawner::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Parameters;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADW_RunnerCharacter* Runner = GetWorld()->SpawnActor<ADW_RunnerCharacter>(RunnerClass, GetActorLocation(), GetActorRotation(), Parameters);
	Runner->CurrentType = RunnerType;
	
}

// Called every frame
void ADW_RunnerSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

