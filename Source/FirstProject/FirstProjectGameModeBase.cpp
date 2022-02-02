// Copyright Epic Games, Inc. All Rights Reserved.


#include "FirstProjectGameModeBase.h"


AFirstProjectGameModeBase::AFirstProjectGameModeBase()
{
	AmountAlive = 0;
	CurrentRound = 1;

	// Initialize as true to spawn on first round
	bShouldSpawn = true;
}
