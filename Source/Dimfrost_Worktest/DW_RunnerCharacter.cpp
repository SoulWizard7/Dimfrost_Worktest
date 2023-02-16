// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_RunnerCharacter.h"

#include "DW_AIController.h"
#include "DW_GameManager.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADW_RunnerCharacter::ADW_RunnerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_RunnerCharacter::BeginPlay()
{	
	Super::BeginPlay();

	SpawnPosition = GetActorLocation();
	
	GameManager = Cast<ADW_GameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADW_GameManager::StaticClass()));
	
	if(GameManager != nullptr)
	{
		GameManager->RunnerList.Add(this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 20.f, FColor::Red, FString::Printf(TEXT("No find Gamemanger")) , true, FVector2D(1.f));
	}
}

void ADW_RunnerCharacter::SetBlackBoardValues()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if(NavSys)
	{
		FVector PoleLoc = FVector::ZeroVector;
		NavSys->K2_GetRandomLocationInNavigableRadius(GetWorld(), GameManager->GetActorLocation(), PoleLoc, 120.f);
		BlackBoard->SetValueAsVector("PoleLocation", PoleLoc);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 20.f, FColor::Red, FString::Printf(TEXT("NavSys is nullptr")) , true, FVector2D(1.f));
	}

	BlackBoard->SetValueAsObject("PlayerActor", GetWorld()->GetFirstPlayerController()->GetPawn());		
}

void ADW_RunnerCharacter::SetBlackboard()
{
	BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner());
}

void ADW_RunnerCharacter::ActivateRunner()
{
	CurrentState = ERunnerState::Active;

	if(BlackBoard == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("blackboard null")) , true, FVector2D(1.f));
		return;
	}
	
	BlackBoard->SetValueAsEnum("RunnerState", static_cast<uint8>(ERunnerState::Active));
	BlackBoard->SetValueAsEnum("RunnerType", static_cast<uint8>(CurrentType));

	if(CurrentType == ERunnerType::Camper)
	{
		CalculatePathLenght();	
	}	
}

 void ADW_RunnerCharacter::CalculatePathLenght()
{
	RunnerPathToPoleDist = 0.f;	
	FVector EndPos = GameManager->GetActorLocation();
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), EndPos);

	for (int i = 0; i < path->PathPoints.Num() - 1; ++i)
	{
		float dist = FVector::Dist2D(path->PathPoints[i], path->PathPoints[i+1]);
		RunnerPathToPoleDist += dist;
	}

	RunnerPathToPoleDist += GameManager->CamperTriggerDistOffset;
}

void ADW_RunnerCharacter::ChangeRunnerType(ERunnerType NewType)
{
	if(BlackBoard == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("blackboard null")) , true, FVector2D(1.f));
		return;
	}
	
	CurrentType = NewType;	
	BlackBoard->SetValueAsEnum("RunnerType", static_cast<uint8>(NewType));	
}

void ADW_RunnerCharacter::ChangeRunnerState(ERunnerState NewState)
{
	if(BlackBoard == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("blackboard null")) , true, FVector2D(1.f));
		return;
	}
	
	CurrentState = NewState;
	BlackBoard->SetValueAsEnum("RunnerState", static_cast<uint8>(NewState));	
}

void ADW_RunnerCharacter::Rush()
{
	if(CurrentState != ERunnerState::MoveToPole)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("Rush")) , true, FVector2D(1.f));
		ChangeRunnerState(ERunnerState::MoveToPole);
	}
}

void ADW_RunnerCharacter::StopBehaviour()
{
	ChangeRunnerState(ERunnerState::Waiting);
	GameManager->RunnerEliminated();
	ADW_AIController* AIController = Cast<ADW_AIController>(UAIBlueprintHelperLibrary::GetAIController(this));
	AIController->StopBehaviour();
}

void ADW_RunnerCharacter::ResetRunner()
{
	ChangeRunnerState(ERunnerState::Waiting);
	SetActorLocation(SpawnPosition);
	ResetRagdoll();
	bIsEliminated = false;
	ADW_AIController* AIController = Cast<ADW_AIController>(UAIBlueprintHelperLibrary::GetAIController(this));
	AIController->StartBehaviour();
}

// Called every frame
void ADW_RunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentType == ERunnerType::Camper && CurrentState == ERunnerState::Active)
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("Player to pole dist: %f "), GameManager->PlayerToPoleDist) , true, FVector2D(1.f));
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, FString::Printf(TEXT("Runner Path To Pole Dist: %f "), RunnerPathToPoleDist) , true, FVector2D(1.f));
		
		if(GameManager->PlayerToPoleDist > RunnerPathToPoleDist)
		{
			Rush();
		}
	}	
}

// Called to bind functionality to input
void ADW_RunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

