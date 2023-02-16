// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_AIController.h"

#include "DW_RunnerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


// Sets default values
ADW_AIController::ADW_AIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

// Called when the game starts or when spawned
void ADW_AIController::BeginPlay()
{
	Super::BeginPlay();
		
}

void ADW_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ADW_RunnerCharacter* chr = Cast<ADW_RunnerCharacter>(InPawn);

	if(chr != nullptr)
	{
		BlackboardComponent->InitializeBlackboard(*chr->BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*chr->BehaviorTree);
		chr->SetBlackboard();		
		
		if(chr->BehaviorTree == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("BehaviorTree null")) , true, FVector2D(1.f));
		}

		if(chr->BlackBoard == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("blackboard null")) , true, FVector2D(1.f));
		}		
	}	
}

void ADW_AIController::StartBehaviour()
{	
	BehaviorTreeComponent->StartLogic();
}

void ADW_AIController::StopBehaviour()
{
	BehaviorTreeComponent->StopLogic("Eliminated");
}

// Called every frame
void ADW_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


