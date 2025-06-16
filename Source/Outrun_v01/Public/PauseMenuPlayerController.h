// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PauseUI.h"
#include "SaveLoad.h"
#include "PauseMenuPlayerController.generated.h"

class ASaveLoad;

/**
 * This class contains parameters and functions used to display and hide the pause menu when
 * the player presses the corresponding button assigned in the MainCharacter class.
 * In addition to this, it will pause gameplay whilst the menu is visible, so that gameplay 
 * no longer progresses whilst the player is unable to make any further inputs except for the
 * buttons assigned to the widget.
 */
UCLASS()
class OUTRUN_V01_API APauseMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveLoad")
	class ASaveLoad* PauseSaveLoadGame;

	

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HidePauseMenu();

	
	void TogglePauseMenu();

	
	void PauseMenuLoadGame();

	
	void PauseMenuSaveGame();

};
