// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AFirstProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	AFirstProjectGameModeBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 AmountAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 CurrentRound;
	
};
