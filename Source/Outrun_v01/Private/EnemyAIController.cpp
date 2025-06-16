// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	Player1Pawn = Cast<AMainCharacter>(UGameplayStatics::UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player2Pawn = Cast<AMainCharacter>(UGameplayStatics::UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	
	if (Enemy->GetDistanceTo(Player1Pawn) < 800 && !(Player1Pawn->bIsCrouched && !Player1Pawn->GetIsDashing()))
	{
		MoveToActor(Player1Pawn);
	}
	else
	{
		StopMovement();
	}
}
