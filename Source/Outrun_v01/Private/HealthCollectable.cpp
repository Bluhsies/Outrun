// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCollectable.h"

/*
* HealingAmount is set in the constructor with a default value, however, this can be amended
* in the inspector for individual health collectables.
*/

AHealthCollectable::AHealthCollectable()
{

	HealingAmount = 10.f;


}

/*
* The OnOverlapBegin function checks to see if the OtherActor, which overlaps with
* the health collectables, is an instance of the MainCharacter. If it is, then it will
* increase the health of the player by the HealingAmount and currently hide the
* actor in the scene, so that they cannot interact with it continuously when they overlap.
*/

void AHealthCollectable::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	UE_LOG(LogTemp, Warning, TEXT("AHealthCollectable::OnOverlapBegin"));


	if (Cast<AMainCharacter>(OtherActor))
	{

		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

		UE_LOG(LogTemp, Warning, TEXT("Healthpickedup"));

		MainChar->SetHealth(MainChar->GetHealth() + HealingAmount);

		this->SetActorHiddenInGame(true);

	}

	Super::OnOverlapBegin(OverlappedActor, OtherActor);

}

/*
* The OnOverlapEnd function is similar to the OnOverlapBegin, but will instead make the
* actor unhidden in the game.
*/

void AHealthCollectable::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{

	/*UE_LOG(LogTemp, Warning, TEXT("AHealthCollectable::OnOverlapEnd"));

	AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

	if (MainChar)
	{

		this->SetActorHiddenInGame(false);

	}*/

}
