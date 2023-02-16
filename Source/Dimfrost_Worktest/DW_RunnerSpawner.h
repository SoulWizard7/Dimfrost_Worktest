// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_RunnerCharacter.h"
#include "GameFramework/Actor.h"
#include "DW_RunnerSpawner.generated.h"

UCLASS()
class DIMFROST_WORKTEST_API ADW_RunnerSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_RunnerSpawner();

	UPROPERTY(EditDefaultsOnly, Category=Runner)
	TSubclassOf<ADW_RunnerCharacter> RunnerClass;

	UPROPERTY(EditAnywhere, Category=Runner)
	ERunnerType RunnerType = ERunnerType::Camper;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
