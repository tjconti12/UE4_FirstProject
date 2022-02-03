// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "Main.h"
#include "Components/SphereComponent.h"
#include "MainPlayerController.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));

	RootComponent = Mesh;

	CollisionVolume->SetupAttachment(GetRootComponent());

	// CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);

	// Get Ref to Game Mode
	AMainGameMode* Ref = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Ref)
	{
		GameRef = Ref;
	}
	else 
	{
		GameRef = nullptr;
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameRef->bDoorOpen)
	{
		OpenDoor();
	}
}

void ADoor::OpenDoor()
{
	Destroy();
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		if (Main->MainPlayerController)
		{
			Main->MainPlayerController->DisplayInfoWidget();
		}
		Main->OverlappingDoor = true;
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		if (Main->MainPlayerController)
		{
			Main->MainPlayerController->RemoveInfoWidget();
		}
		Main->OverlappingDoor = false;
	}
}