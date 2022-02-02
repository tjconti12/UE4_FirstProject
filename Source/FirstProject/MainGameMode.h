// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AMainGameMode();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 AmountAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 CurrentRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 NumSpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	int32 SpawnQuery;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	bool bShouldSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	bool bDoneSpawning;

	// To determine if the other spawn points should be used once the door is open
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	bool bDoorOpen;

	
	// The spawn points that can be selected in BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class ASpawnVolume> Spawn_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ASpawnVolume> Spawn_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ASpawnVolume> Spawn_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ASpawnVolume> Spawn_4;

	// Array to hold the spawn points if they are valid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<ASpawnVolume*> SpawnArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ASpawnVolume> VolumeToSpawn;

	UFUNCTION()
	void InitialSpawn();

	UFUNCTION()
	void TriggerSpawn();
};
