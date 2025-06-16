// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoad.h"
#include "MainMenu.generated.h"

class UButton;

UCLASS()
class OUTRUN_V01_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

protected:

	/*
	* The exposed UPROPERTY variables below will bind/locate a widget with the same name
	* in the assigned widget blueprint (they must be identical in name) and will action
	* the assigned exposed UFUNCTIONs assigned to it.
	*/

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY()
	ASaveLoad* Loader;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnLoadClicked();

	UFUNCTION()
	void OnQuitClicked();


	// bIsWorking will check to see if the the Initialize function is working and if it is not,
	// it will return false.
	UPROPERTY()
	bool bIsWorking;
	
};
