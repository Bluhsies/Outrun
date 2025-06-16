// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuPlayerController.generated.h"

class UMainMenu;

/*
* The MainMenuPlayerController class allows the user to assign the main menu widget
* to the level name Ians_Level. Rather than have to assign the functionality of spawning the
* widget via blueprint, this simply needs to be assigned in the Game Mode to be called instead.
*/

UCLASS()
class OUTRUN_V01_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	
	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:

	UMainMenu* Main;
	

};
