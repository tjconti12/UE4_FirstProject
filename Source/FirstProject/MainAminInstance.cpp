// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAminInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Main.h"

void UMainAminInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		// Will get the owner of this animation instance
		// Which pawn owns this AnimInstance
		// Will initialize it if its a nullptr (will happen on first time)
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}

void UMainAminInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		// Will get the owner of this animation instance
		// Which pawn owns this AnimInstance
		// Will initialize it if its a nullptr (will happen on first time)
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		// Want to get the speed and if character is in the air
		FVector Speed = Pawn->GetVelocity();
		
		// If Character is not jumping, we only want its X and Y speed (set the Z to 0)
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		
		// Need to get magnitude of the vector, since Movement speed is just a float
		// Size() gives us the magnitude of the vector as a float
		MovementSpeed = LateralSpeed.Size();

		// The Pawn MovementComponent has a function to see if the pawn is falling (in the air)
		// Then sets the bIsInAir appropriately 
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (Main == nullptr)
		{
			Main = Cast<AMain>(Pawn);
		}


	}
}