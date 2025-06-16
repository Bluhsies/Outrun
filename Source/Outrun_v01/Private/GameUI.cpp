// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI.h"
#include "Components/ProgressBar.h"
#include "MainCharacter.h"


void UGameUI::UpdateHealthBar(float HealthPercent, AMainCharacter* Character)
{

	
	OwnerCharacter = Character;
	

	if (HealthBar && OwnerCharacter)
	{

		HealthBar->SetPercent(OwnerCharacter->GetHealth() / OwnerCharacter->MaxHealth);

	}
	
	
}

void UGameUI::UpdateScore(AMainCharacter* Character)
{

	OwnerCharacter = Character;

	if (ScoreText && OwnerCharacter)
	{

		float ScoreValue = OwnerCharacter->GetScore();

		FText ScoreTextDisplay = FText::AsNumber(ScoreValue);

		ScoreText->SetText(ScoreTextDisplay);

	}

}





