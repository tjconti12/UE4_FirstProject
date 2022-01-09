// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// #include "Engine/World.h";

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom 
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Second Argument is the socket for the camera boom
	// Let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera should follow the boom, not the controller

	// Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Checks to see if the PlayerInputComponent is valid
	// If not, code gets haulted here
	check(PlayerInputComponent);

	// Add Jump Action
	// IE_Pressed is the button pressed
	// ACharacter already has a jump built in
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	// For mouse controll
	// Pawn already has a function for this
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// For the arrow keys to look around
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

}

// Called to Move Character Forward
void AMain::MoveForward(float Value)
{
	// Good idea to check to make sure the Characters Controller is not a null pointer
	// And to check that Value is not 0 (the button is being pressed)
	if (Controller != nullptr && Value != 0.0f)
	{
		// Gets the direction the controller is facing
		const FRotator Rotation = Controller->GetControlRotation();
		// Pitch and Roll are zeroed out, with new yaw value
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Rotation Matrix has information about the x, y, and z axis of a particular rotation
		// EAxis::X gets the forward vector from the rotation
		// This will be the direction we are looking with the camera
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// Adds movement in the Direction we are facing, for the Value passed in (will be 1.0)
		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value)
{
	// Good idea to check to make sure the Characters Controller is not a null pointer
	// And to check that Value is not 0 (the button is being pressed)
	if (Controller != nullptr && Value != 0.0f)
	{
		// Gets the direction the controller is facing
		const FRotator Rotation = Controller->GetControlRotation();
		// Pitch and Roll are zeroed out, with new yaw value
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Rotation Matrix has information about the x, y, and z axis of a particular rotation
		// EAxis::X gets the forward vector from the rotation
		// This will be the direction we are looking with the camera
		// Y will return the right vector (to move right/left)
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// Adds movement in the Direction we are facing, for the Value passed in (will be 1.0)
		AddMovementInput(Direction, Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	// Takes input and rotates in the yaw axis
	// Called every frame
	// Up to 65 degrees per second (65 is initialized for BaseTurnRate)
	// Get Delta Seconds from World (will produce a rate per second)
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
	// Takes input and rotates in the pitch axis
	// Called every frame
	// Up to 65 degrees per second (65 is initialized for BaseLookUpRate)
	// Get Delta Seconds from World (will produce a rate per second)
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}