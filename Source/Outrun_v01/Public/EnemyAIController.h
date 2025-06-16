// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class OUTRUN_V01_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	AMainCharacter* Player1Pawn;
	AMainCharacter* Player2Pawn;
	
};
