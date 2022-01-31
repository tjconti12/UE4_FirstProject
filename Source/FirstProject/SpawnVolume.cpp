// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"
#include "TimerManager.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

	NumToSpawn = 1;

	bDoneSpawning = false;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	
	return Point;
}
// The _Implementation is to tell the engine that this is the implementation in C++ (due to the UFUNCTION macro BlueprintNativeEvent)
void ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	for (int i = 0; i < NumToSpawn; i++)
	{
		if (ToSpawn)
		{
			UWorld* World = GetWorld();
			FActorSpawnParameters SpawnParams;

			if (World)
			{
				FVector Extent = SpawningBox->GetScaledBoxExtent();
				FVector Origin = SpawningBox->GetComponentLocation();

				FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
				AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Point, FRotator(0.f), SpawnParams);

				AEnemy* Enemy = Cast<AEnemy>(Actor);
				if (Enemy)
				{
					Enemy->SpawnDefaultController();

					AAIController* AICont = Cast<AAIController>(Enemy->GetController());
					if (AICont)
					{
						Enemy->AIController = AICont;
					}
				}
			}
		}
	}
}

void ASpawnVolume::CompleteSpawn(UClass* ToSpawn, const FVector& Location)
{
	
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);

		return SpawnArray[Selection];
	}
	else 
	{
		return nullptr;
	}
}