// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "SpawnVolume.h"

AMainGameMode::AMainGameMode()
{
	AmountAlive = 0;
	CurrentRound = 1;
	NumSpawned = 0;

	// Initialize as true to spawn on first round
	bShouldSpawn = true;

	if (Spawn_1)
	{
		SpawnArray.Add(Spawn_1);
	}
	if (Spawn_2)
	{
		SpawnArray.Add(Spawn_2);
	}
	if (Spawn_3 && bDoorOpen)
	{
		SpawnArray.Add(Spawn_3);
	}
	if (Spawn_4 && bDoorOpen)
	{
		SpawnArray.Add(Spawn_4);
	}
}


void AMainGameMode::Tick(float DeltaTime)
{
	if (bShouldSpawn)
	{
		/*int32 random = FMath::RandRange(0, SpawnArray.Num() - 1)*/;

		int32 random = 0;
		
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

		
		// Increment the num spawned
		NumSpawned++;

	}
	
}
