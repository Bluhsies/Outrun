// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameUI.generated.h"



class UProgressBar;
class A
	;
/**
 * 
 */
UCLASS()
class OUTRUN_V01_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

public:


	UFUNCTION()
	void UpdateHealthBar(float HealthPercent, AMainCharacter* Character);

	UFUNCTION()
	void UpdateScore(AMainCharacter* Character);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	AMainCharacter* OwnerCharacter;

	
	
};
