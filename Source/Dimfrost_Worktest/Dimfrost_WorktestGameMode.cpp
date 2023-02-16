// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dimfrost_WorktestGameMode.h"
#include "Dimfrost_WorktestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADimfrost_WorktestGameMode::ADimfrost_WorktestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
