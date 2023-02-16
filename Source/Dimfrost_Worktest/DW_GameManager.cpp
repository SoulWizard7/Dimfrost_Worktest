// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_GameManager.h"

#include "Dimfrost_WorktestCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADW_GameManager::ADW_GameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_GameManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<ADimfrost_WorktestCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (PlayerRef == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("player ref null")) , true, FVector2D(1.f));
	}	
}

// Called every frame
void ADW_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bGameHasStarted) return;

	PlayerToPoleDist = FVector::Dist2D(GetActorLocation(), PlayerRef->GetActorLocation());

	if(PlayerRef != nullptr)
	{
		if(!bRusherRush)
		{			
			if (PlayerToPoleDist > DistForRushersToRush)
			{
				RushersRush();			
			}
		}		
	}

	if(!bOpportunistRush)
	{
		for (int i = 0; i < RunnerList.Num(); ++i)
		{		
			float RunnerToPoleDist = FVector::Dist2D(GetActorLocation(), RunnerList[i]->GetActorLocation());
			if(RunnerToPoleDist < DistForOpportunistsToRush)
			{
				OpportunistRush();
			}
		}
	}		
}

void ADW_GameManager::StartGame()
{
	RunnersEliminated = 0;
	FVector polePos = GetActorLocation();
	for (int i = 0; i < RunnerList.Num(); ++i)
	{		
		//RunnerList[i]->SetPoleLocation(polePos);
		RunnerList[i]->SetBlackBoardValues();
		
		if(RunnerList[i]->CurrentType == ERunnerType::Random)
		{
			uint8 random = FMath::RandRange(1, 5);			
			RunnerList[i]->ChangeRunnerType(static_cast<ERunnerType>(random));
		}
	}
	ResetPlayer();
	ResetRunners();

	for (int i = 0; i < RunnerList.Num(); ++i)
	{
		RunnerList[i]->ActivateRunner();
	}
	bGameHasStarted = true;
}

void ADW_GameManager::ResetRunners()
{
	for (int i = 0; i < RunnerList.Num(); ++i)
	{
		RunnerList[i]->ResetRunner();
	}
}

void ADW_GameManager::ResetPlayer()
{
	PlayerRef->ResetPlayerPosition();
}


void ADW_GameManager::RushersRush()
{
	bRusherRush = true;
	for (int i = 0; i < RunnerList.Num(); ++i)
	{
		if(RunnerList[i]->CurrentType == ERunnerType::Rusher)
		{
			RunnerList[i]->Rush();
		}		
	}
}

void ADW_GameManager::OpportunistRush()
{
	bOpportunistRush = true;
	for (int i = 0; i < RunnerList.Num(); ++i)
	{
		if(RunnerList[i]->CurrentType == ERunnerType::Opportunist)
		{
			RunnerList[i]->Rush();
		}		
	}
}

void ADW_GameManager::RunnerEliminated()
{
	RunnersEliminated++;
	if(RunnersEliminated >= EliminationsForHidersToRush)
	{
		for (int i = 0; i < RunnerList.Num(); ++i)
		{
			if(RunnerList[i]->CurrentType == ERunnerType::Hider)
			{
				RunnerList[i]->ChangeRunnerType(ERunnerType::Rusher);
			}		
		}
		bRusherRush = false;
	}
	if(RunnersEliminated >= RunnerList.Num())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 60.f, FColor::Green, FString::Printf(TEXT("YOU WIN!!")) , true, FVector2D(1.f));
	}
}

