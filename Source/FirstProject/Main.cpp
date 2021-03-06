// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// #include "Engine/World.h";
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "Door.h"
#include "MainGameMode.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"

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

	// Set Capsule Radius and Height (for collision capsule)
	GetCapsuleComponent()->SetCapsuleSize(28.f, 92.f);

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom 
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Second Argument is the socket for the camera boom
	// Let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera should follow the boom, not the controller

	// Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// Prevents character from moving with the controller (only want camera to move)
	// boolean is inherited from class
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 840.f, 0.0f);  // ... at this rotation rate (0 pitch, 540 yaw, 0 roll)
	GetCharacterMovement()->JumpZVelocity = 550.f; // Set the jump velocity
	GetCharacterMovement()->AirControl = 0.2f; // allows small amount of control while in the air

	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 150.f;
	Stamina = 120.f;
	Coins = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;
	bLMBDown = false;

	bActionDown = false;

	bESCDown = false;

	OverlappingDoor = false;


	// Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bCycleAttack = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;

	CurrentLevel = 1;

	NumKilledThisLevel = 0;

	bMoveToNext = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());
	
	// Testing
	GameRef = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Testing
	if (GameRef)
	{
		Alive = GameRef->AmountAlive;
		NumSpawned = GameRef->NumSpawned;
		bDoneSpawning = GameRef->bDoneSpawning;
	}
	//////////////

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			Stamina -= DeltaStamina;
			if (Stamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			if (bMovingForward || bMovingRight)
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else {
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else 
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else 
			{
				Stamina -= DeltaStamina;
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
				
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else // Shift key up 
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else 
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}

	// Check to see if we should increment level
	if (NumKilledThisLevel == CurrentLevel)
	{
		CurrentLevel++;
		NumKilledThisLevel = 0;
		
		// This allows us to start spawning again in the EnemyVolume_BP
		bMoveToNext = true;
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0);
	return LookAtRotationYaw;
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMain::ActionDown);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AMain::ActionUp);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	// For mouse control
	// Pawn already has a function for this
	// Calling Main version now, that calls the pawn version of the function
	PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);

	// For the arrow keys to look around
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

}

bool AMain::CanMove(float Value)
{
	if (MainPlayerController)
	{
		return (Value != 0.0f) && 
			(MovementStatus != EMovementStatus::EMS_Dead) && 
			!MainPlayerController->bPauseMenuVisible;
	}
	return false;
}

void AMain::Turn(float Value)
{
	if (CanMove(Value))
	{
		AddControllerYawInput(Value);
	}
}


void AMain::LookUp(float Value)
{
	if (CanMove(Value))
	{
		AddControllerPitchInput(Value);
	}
}

// Called to Move Character Forward
void AMain::MoveForward(float Value)
{
	// MoveForward gets called every frame. So we set it to false, and it will only change to true if there is a controller input
	bMovingForward = false;
	// Good idea to check to make sure the Characters Controller is not a null pointer
	// And to check that Value is not 0 (the button is being pressed)
	if (CanMove(Value))
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

		bMovingForward = true;
	}
}

void AMain::MoveRight(float Value)
{
	bMovingRight = false;
	// Good idea to check to make sure the Characters Controller is not a null pointer
	// And to check that Value is not 0 (the button is being pressed)
	if (CanMove(Value))
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
		bMovingRight = true;
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

void AMain::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;

	if (EquippedWeapon)
	{
		if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
		{
			Attack();
		}
	}
}
void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::ActionDown()
{
	bActionDown = true;
	if (OverlappingDoor && Coins >= 500)
	{
		// AMainGameMode* GameRef = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameRef)
		{
			GameRef->bDoorOpen = true;
			Coins -= 500;
		}
	}
	else
	{
		if (ActiveOverlappingItem)
		{
			AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
			if (Weapon && (Coins >= Weapon->Cost))
			{
				Weapon->Equip(this);
				Coins -= Weapon->Cost;
				SetActiveOverlappingItem(nullptr);
				MainPlayerController->RemoveWeaponWidget();
			}
		}
	}
}

void AMain::ActionUp()
{
	bActionDown = false;
}


void AMain::ESCDown()
{
	bESCDown = true;
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void AMain::ESCUp()
{
	bESCDown = false;
}

void AMain::DecrementHealth(float Amount)
{
	Health -= Amount;
	if (Health <= 0.f)
	{
		
		Die();
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::IncrementHealth(float Amount)
{
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.2f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMain::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::Jump()
{
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::ShowPickupLocations()
{
	/* Standard For loop
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
	*/
	
	// Range Version of the For loop
	for (FVector Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	bAttacking = true;
	SetInterpToEnemy(true);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		if (bCycleAttack) {
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
			bCycleAttack = !bCycleAttack;
		}
		else {
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			bCycleAttack = !bCycleAttack;
		}

	
		
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	SetActiveOverlappingItem(nullptr);
	SetInterpToEnemy(false);
}

void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
	
}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);

	return DamageAmount;
}

void AMain::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);
	
	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}
	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector MainLocation = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - MainLocation).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				Enemy->bIsTarget = false;
				float DistanceToActor = (Enemy->GetActorLocation() - MainLocation).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
			
		}
		if (MainPlayerController)
		{
			/*MainPlayerController->DisplayEnemyHealthBar();*/
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
		ClosestEnemy->bIsTarget = true;
	}
	
}

void AMain::SaveGame()
{
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.Round = GameRef->CurrentRound;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
	
	if (EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}


void AMain::LoadGame()
{
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	Health = LoadGameInstance->CharacterStats.Health;
	Coins = LoadGameInstance->CharacterStats.Coins;
	GameRef->CurrentRound = LoadGameInstance->CharacterStats.Round;
	Stamina = LoadGameInstance->CharacterStats.Stamina;

	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			if (Weapons->WeaponMap.Contains(WeaponName))
			{
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);
			}
		}
	}
	

	SetActorLocation(LoadGameInstance->CharacterStats.Location);
	SetActorRotation(LoadGameInstance->CharacterStats.Rotation);

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainPlayerController->RemovePauseMenu();
}