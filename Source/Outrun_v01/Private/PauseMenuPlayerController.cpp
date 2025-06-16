// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad.h"
#include "TimerManager.h"


/*
* In BeginPlay, we check to see if WPauseMenu is true; which is a TSubclass of UUserWidget.
* If it is, then we create a widget assigned to variable PauseMenu, outlined in the .h file.
* If PauseMenu is then true, it will add the pause menu to the viewport and hide it from the
* user's view until it has been called based on the functions below.
* 
* In addition to this, we create an instance of UPauseUI which allows to assign a button in the
* widget blueprint created from this class with the name "ResumeButton" and add the on click
* event to the button, without the need for blueprints to be added.
* 
* When the assigned button is pressed, it will call the HidePauseMenu function.
*/

void APauseMenuPlayerController::BeginPlay()
{

	Super::BeginPlay();

	
	
	if (!PauseSaveLoadGame)
	{
		PauseSaveLoadGame = GetWorld()->SpawnActor<ASaveLoad>();
	}


	ACharacter* CharacterNew = GetCharacter();
	if (CharacterNew)
	{
		if (WPauseMenu)
		{

			PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

			if (PauseMenu)
			{

				PauseMenu->AddToViewport();
				PauseMenu->SetVisibility(ESlateVisibility::Hidden);


				UPauseUI* PauseWidget = Cast<UPauseUI>(PauseMenu);
				if (PauseWidget->ResumeButton)
				{

					PauseWidget->ResumeButton->OnClicked.AddDynamic(this, &APauseMenuPlayerController::HidePauseMenu);

				}

				if (PauseWidget->LoadButton)
				{

					PauseWidget->LoadButton->OnClicked.AddDynamic(this, &APauseMenuPlayerController::PauseMenuLoadGame);

				}

				if (PauseWidget->SaveButton)
				{

					PauseWidget->SaveButton->OnClicked.AddDynamic(this, &APauseMenuPlayerController::PauseMenuSaveGame);

				}

				

			}



		}

	}

}

/*
* The DisplayPauseMenu function will check to see if PauseMenu is true.
* If it is, it will change the value of bPauseMenuVisible to true, which is a boolean,
* signaling to the TogglePauseMenu function whether it should be visible or hidden.
* It will then make the menu widget visible and call the SetPause(true) function
* to pause gameplay.
* It then makes an instance of FInputModeGameAndUI to allow user input when the menu
* is visible and allows the mouse cursor to be visible.
*/

void APauseMenuPlayerController::DisplayPauseMenu()
{

	if (PauseMenu)
	{

		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		SetPause(true);

		UE_LOG(LogTemp, Warning, TEXT("Game Paused"));

		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UE_LOG(LogTemp, Warning, TEXT("Pause menu displayed."));
	}

}

/*
* The HidePauseMenu function will check to see if PauseMenu is true.
* If it is, then it will change the status of the SetPause function to false and resume
* gameplay.
* The widget menu will be hidden and the variable bPauseMenuVisislbe set to false, which is
* used in the TogglePauseMenu function to determine if the menu should be visible or not.
* It will then hide the mouse cursor, so as not to impact on the user's gameplay.
*/

void APauseMenuPlayerController::HidePauseMenu()
{
	
	if (PauseMenu)
	{

		SetPause(false);

		UE_LOG(LogTemp, Warning, TEXT("Game Unpaused"));

		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		bPauseMenuVisible = false;
	
		FInputModeGameAndUI InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

		UE_LOG(LogTemp, Warning, TEXT("Pause menu hidden."));
	}

}

/*
* The TogglePauseMenu function checks to see if bPauseMenuVisible is true or false.
* This function may not be required in the final installment of the game.
*/

void APauseMenuPlayerController::TogglePauseMenu()
{

	UE_LOG(LogTemp, Warning, TEXT("TogglePauseMenu triggered"));

	if (bPauseMenuVisible)
	{

		HidePauseMenu();

	}
	else
	{

		DisplayPauseMenu();

	}

}

/*
* This function calls an instance of the LoadGame function from the SaveLoad class, but assigns
* it to the pause menu.
*/

void APauseMenuPlayerController::PauseMenuLoadGame()
{

	UE_LOG(LogTemp, Warning, TEXT("Loaded"));

	if (PauseSaveLoadGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameLoaded"));
		
		PauseSaveLoadGame->LoadGame();

	}
	
}

/*
* This function calls an instance of the SaveGame function from the SaveLoad class, but is used
* as part of the pause menu.
*/

void APauseMenuPlayerController::PauseMenuSaveGame()
{

	if (PauseSaveLoadGame)
	{

		UE_LOG(LogTemp, Warning, TEXT("GameSaved"));

		PauseSaveLoadGame->SaveGame();

	}
	

}
