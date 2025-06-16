// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "MainMenu.h"
#include "Blueprint/UserWidget.h"

/*
The information called in the BeginPlay function creates the functionality which is similar to
setting it up via blueprint, but adds it to the viewport and sets the input mode,
when the game starts.
This means that a blueprint did not need to be setup to do the same thing.
*/

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{

		Main = CreateWidget<UMainMenu>(this, MainMenuWidgetClass);
		if (Main)
		{

			Main->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

		}

	}
	
}



