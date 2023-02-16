// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_RunnerCharacter.h"
#include "GameFramework/Actor.h"
#include "DW_GameManager.generated.h"

class ADimfrost_WorktestCharacter;

UCLASS()
class DIMFROST_WORKTEST_API ADW_GameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_GameManager();

	UPROPERTY(EditAnywhere, Category=GameRules)
	float DistForRushersToRush = 4000.f;

	UPROPERTY(EditAnywhere, Category=GameRules)
	float DistForOpportunistsToRush = 4000.f;

	UPROPERTY(EditAnywhere, Category=GameRules)
	int EliminationsForHidersToRush = 2;

	UPROPERTY(EditAnywhere, Category=GameRules)
	float CamperTriggerDistOffset = 0.f;

	UPROPERTY()
	ADimfrost_WorktestCharacter* PlayerRef;

	UPROPERTY()
	float PlayerToPoleDist = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bGameHasStarted = false;

	UPROPERTY(BlueprintReadOnly)
	bool bOpportunistRush = false;

	UPROPERTY(BlueprintReadOnly)
	bool bRusherRush = false;

	UPROPERTY(BlueprintReadOnly)
	int RunnersEliminated = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<ADW_RunnerCharacter*> RunnerList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void ResetRunners();

	UFUNCTION(BlueprintCallable)
	void ResetPlayer();

	UFUNCTION()
	void RushersRush();

	UFUNCTION()
	void OpportunistRush();

	UFUNCTION(BlueprintCallable)
	void RunnerEliminated();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerWinsRound();
};
