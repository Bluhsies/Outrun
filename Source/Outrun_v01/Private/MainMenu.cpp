// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

/*
The Initialize function will check to see which button in the main menu widget has been clicked on by the user.
When one of the buttons is clicked on, it will call the appropriate function for that button.
For example, if the StartButton is clicked on during play, the Initialize function will check to see
if the start button has been clicked and called the OnStartClicked() function.
For now, this will produce a warning entry in the log until the first level is selected.
*/

bool UMainMenu::Initialize()
{
	
	bIsWorking = Super::Initialize();
	if (!bIsWorking)
	{
		return false;
	}

	
	if (StartButton)
	{

		StartButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartClicked);

	}
	
	if (LoadButton)
	{

		LoadButton->OnClicked.AddDynamic(this, &UMainMenu::OnLoadClicked);

	}
	
	if (SettingsButton)
	{

		SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartClicked);

	}
	
	if (QuitButton)
	{

		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);

	}

	return true;

}

void UMainMenu::OnStartClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked"));

	UGameplayStatics::OpenLevel(this, FName("Final_Game"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

}

void UMainMenu::OnLoadClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Load Button Clicked"));



	if (Loader)
	{
		Loader->LoadGame();
		UGameplayStatics::OpenLevel(this, FName("TestLevel"));
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loader is null. Cannot load game."));
	}





}

void UMainMenu::OnQuitClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked"));

}


