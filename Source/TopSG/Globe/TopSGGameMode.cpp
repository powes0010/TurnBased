// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopSGGameMode.h"
#include "Controller/TopSGPlayerController.h"
#include "Character/TopSGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopSGGameMode::ATopSGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopSGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Character/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}