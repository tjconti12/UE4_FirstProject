// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "SpawnVolume.h"

AMainGameMode::AMainGameMode()
{
	AmountAlive = 0;
	CurrentRound = 1;
	NumSpawned = 0;
	SpawnQuery = 0;

	bDoneSpawning = false;

	// Initialize as true to spawn on first round
	bShouldSpawn = true;

	// Call to get the first spawn point
	InitialSpawn();
}


void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AmountAlive == 0 && bDoneSpawning)
	{
		CurrentRound++;
		bDoneSpawning = false;
		bShouldSpawn = true;
	}
	
}

void AMainGameMode::InitialSpawn()
{
	int32 random = FMath::RandRange(1, 2);

	switch (random)
	{
	case 0:
		VolumeToSpawn = Spawn_1;
		break;
	case 1:
		VolumeToSpawn = Spawn_2;
		break;
	case 2:
		VolumeToSpawn = Spawn_3;
		break;
	case 3:
		VolumeToSpawn = Spawn_4;
		break;
	default:
		break;
	}
}

void AMainGameMode::TriggerSpawn()
{
	if (bShouldSpawn)
	{
		int32 random = FMath::RandRange(1, 2);
		
		switch (random)
		{
		case 0:
			VolumeToSpawn = Spawn_1;
			break;
		case 1:
			VolumeToSpawn = Spawn_2;
			break;
		case 2:
			VolumeToSpawn = Spawn_3;
			break;
		case 3:
			VolumeToSpawn = Spawn_4;
			break;
		default:
			break;
		}
		
	}
}
