// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreCollectable.h"

AScoreCollectable::AScoreCollectable()
{

	Score = 1.f;

}

void AScoreCollectable::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	if (Cast<AMainCharacter>(OtherActor))
	{

		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

		MainChar->SetScore(MainChar->GetScore() + Score);

		Destroy();

	}

}


