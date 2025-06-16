// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"


class UButton;
/**
 * The PauseUI class is used to make a widget blueprint to assign the buttons
 * for player interaction. This means that a blueprint does not need to be used
 * to call the functions, as the assigned buttons, in conjunction with the PauseMenuPlayerController
 * will action the desired outcome.
 * For example, ResumeButton is bound to the HidePauseMenu function to hide the pause menu widget
 * and resume gameplay.
 */
UCLASS()
class OUTRUN_V01_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	
	
};
