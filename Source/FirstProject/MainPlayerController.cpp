// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	


	if (HUDOverlayAsset && MapName == "SunTemple")
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	if (WInfoWidget)
	{
		InfoWidget = CreateWidget<UUserWidget>(this, WInfoWidget);
		if (InfoWidget)
		{
			InfoWidget->AddToViewport();
			InfoWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WWeaponWidget)
	{
		WeaponWidget = CreateWidget<UUserWidget>(this, WWeaponWidget);
		if (WeaponWidget)
		{
			WeaponWidget->AddToViewport();
			WeaponWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayInfoWidget()
{
	if (InfoWidget)
	{
		bInfoWidgetVisible = true;
		InfoWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveInfoWidget()
{
	if (InfoWidget)
	{
		bInfoWidgetVisible = false;
		InfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

// bool bWeaponWidgetVisible;

void AMainPlayerController::DisplayWeaponWidget()
{
	if (WeaponWidget)
	{
		bWeaponWidgetVisible = true;
		WeaponWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveWeaponWidget()
{
	bWeaponWidgetVisible = false;
	WeaponWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = false;
		

		FInputModeGameOnly InputModeGameOnly;

		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 150.f;

		FVector2D SizeInViewPort = FVector2D(250, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
}