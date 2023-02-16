// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.34.31933/INCLUDE/xkeycheck.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "DW_RunnerCharacter.generated.h"

UENUM(BlueprintType)
enum class ERunnerType : uint8
{
	None,
	Rusher,
	Hider,
	Ninja,
	Opportunist,
	Camper,
	Random
};

UENUM(BlueprintType)
enum class ERunnerState : uint8
{	
	Waiting,
	MoveToPole,
	Active
};

class ADW_GameManager;

UCLASS()
class DIMFROST_WORKTEST_API ADW_RunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_RunnerCharacter();
	
	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackBoard;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERunnerType CurrentType = ERunnerType::Camper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERunnerState CurrentState = ERunnerState::Waiting;

	UPROPERTY(BlueprintReadOnly)
	ADW_GameManager* GameManager;

	UPROPERTY(BlueprintReadWrite)
	bool bIsEliminated = false;

private:
	UPROPERTY()
	float RunnerPathToPoleDist = 0.f;

	UPROPERTY()
	FVector SpawnPosition = FVector::ZeroVector;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
public:
	
	UFUNCTION()
	void SetBlackBoardValues();
	
	UFUNCTION()
	void SetBlackboard();

	UFUNCTION(BlueprintCallable)
	void ActivateRunner();

	UFUNCTION(BlueprintCallable)
	void ChangeRunnerType(ERunnerType NewType);

	UFUNCTION(BlueprintCallable)
	void ChangeRunnerState(ERunnerState NewState);

	UFUNCTION(BlueprintCallable)
	void Rush();

	UFUNCTION(BlueprintCallable)
	void StopBehaviour();

	UFUNCTION(BlueprintCallable)
	void ResetRunner();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetRagdoll();

	UFUNCTION()
	void CalculatePathLenght();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

