// Copyright Epic Games, Inc. All Rights Reserved.

#include "WraithboundGameMode.h"
#include "WraithboundCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWraithboundGameMode::AWraithboundGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
