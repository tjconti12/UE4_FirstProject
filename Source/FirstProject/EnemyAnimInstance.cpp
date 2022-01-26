// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			// Gives us a reference to the enemy at Initialization
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			// Gives us a reference to the enemy at Initialization
			Enemy = Cast<AEnemy>(Pawn);
		}
	}

	if (Pawn)
	{
		// Want to get the speed of enemy
		FVector Speed = Pawn->GetVelocity();

		// We only want its X and Y speed (set the Z to 0) (since it can't jump)
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		// Need to get magnitude of the vector, since Movement speed is just a float
		// Size() gives us the magnitude of the vector as a float
		MovementSpeed = LateralSpeed.Size();
	}
}