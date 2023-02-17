// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_RunnerCharacter.h"

#include "DW_AIController.h"
#include "DW_GameManager.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void ADW_RunnerCharacter::SetWalkSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void ADW_RunnerCharacter::SetBlackBoardValues(FVector PoleLocation)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if(NavSys)
	{
		FVector PoleLoc = FVector::ZeroVector;
		NavSys->K2_GetRandomLocationInNavigableRadius(GetWorld(), PoleLocation, PoleLoc, 120.f);
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
	// This code uses the built-in navigation, finds path to the pole, and measures the distance of the path
	
	RunnerPathToPoleDist = 0.f;	
	FVector EndPos = GameManager->GetActorLocation();
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), EndPos);

	// The NavigationSystemV1 as I understand it, is the brain of the navigation in UE. With it we can access the current navmesh and request
	// a path between two points. This will return a NavigationPath that includes a vector array of the path points.

	for (int i = 0; i < path->PathPoints.Num() - 1; ++i)
	{
		float dist = FVector::Dist2D(path->PathPoints[i], path->PathPoints[i+1]);
		RunnerPathToPoleDist += dist;
	}

	//There are built in functions for calculating the path lenght, but I like to have some oversight to what is happening,
	// hence we just chech the path length manually with a for loop.

	RunnerPathToPoleDist += GameManager->CamperTriggerDistOffset;

	//In the tick function the values will be compared for triggering the Campers rush. Since the Campers path is calculated, 
	//but not the players, I decided to just have an tweakable parameter to add to the distance to compensate.
	//I felt it was too much to constantly calculate players path length in a tick. For a more accurate result
	//one might consider having the players path be calculated every second or something similar.
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
		float RunnerTimeToPole = RunnerPathToPoleDist / GameManager->RunnerWalkSpeed;
		float PlayerTimeToPole = GameManager->PlayerToPoleDist / GameManager->PlayerWalkSpeed;

		// Values will be measured in second it takes for characters to move at max speed to the pole
				
		if(PlayerTimeToPole > RunnerTimeToPole)
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

