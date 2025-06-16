// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "EnemyBTAIController.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class AEnemy;

UCLASS()
class OUTRUN_V01_API AEnemyBTAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	AMainCharacter* Player1Pawn;
	AMainCharacter* Player2Pawn;
	AEnemy* Enemy;

	float AttackDamageDelayTimer = 0.0f;

	bool HasSeenPlayer(AMainCharacter& Player);
	void HandleAttack(AMainCharacter& Player);
	void SetTargetPlayer();

	bool bHasReachedPlayer = false;
	bool bIsPlayer1Target = false;
	
};
