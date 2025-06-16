// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

void AEnemyBTAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		Player1Pawn = Cast<AMainCharacter>(UGameplayStatics::UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Player2Pawn = Cast<AMainCharacter>(UGameplayStatics::UGameplayStatics::GetPlayerPawn(GetWorld(), 1));

		GetBlackboardComponent()->SetValueAsObject(TEXT("ClosestPlayerActor"), Player1Pawn);

		GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartPosition"), GetPawn()->GetActorLocation());

		Enemy = Cast<AEnemy>(GetPawn());
	}
}

void AEnemyBTAIController::Tick(float DeltaTime)
{
	if (Player1Pawn != nullptr && Player2Pawn != nullptr)
	{
		SetTargetPlayer();

		if (bIsPlayer1Target)
		{
			HandleAttack(*Player1Pawn);
		}
		else
		{
			HandleAttack(*Player2Pawn);
		}

	}

	if (Enemy->bIsAttacking)
	{
		AttackDamageDelayTimer += DeltaTime;
	}
}

bool AEnemyBTAIController::HasSeenPlayer(AMainCharacter& Player)
{
	int Direction = 0;
	bool IsFacingPlayer = false;
	if (Enemy->GetActorRotation().Yaw > 0)
	{
		Direction = 1;
	}
	else
	{
		Direction = -1;
	}

	if (Enemy->GetActorLocation().Z < Player.GetActorLocation().Z - 400) //If player is significantly higher than enemy
	{
		return false;
	}

	//If Enemy is facing towards player, crouching does not help stealth and detection range is bigger
	if (Direction == 1 && Player.GetActorLocation().Y > Enemy->GetActorLocation().Y)
	{
		if (Enemy->GetDistanceTo(&Player) < 1000)
		{
			return true;
		}
	}
	else if (Direction == -1 && Player.GetActorLocation().Y < Enemy->GetActorLocation().Y)
	{
		if (Enemy->GetDistanceTo(&Player) < 1000)
		{
			return true;
		}
	}

	//If player is near, but not facing same way, reduce detection range
	if (Player.bIsCrouched && !Player.GetIsDashing() && Enemy->GetDistanceTo(&Player) < 500)
	{
		return true;
	}

	if (!(Player.bIsCrouched && !Player.GetIsDashing()) && Enemy->GetDistanceTo(&Player) < 800)
	{
		return true;
	}


	return false;
}

void AEnemyBTAIController::HandleAttack(AMainCharacter& Player)
{
	if (Enemy->GetDistanceTo(&Player) < 100 && HasSeenPlayer(Player))
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bHasReachedPlayer"), true);
		if (!Enemy->bIsAttacking)
		{
			AttackDamageDelayTimer = 1.0f;
		}
		if (AttackDamageDelayTimer >= 1.75f)
		{
			Player.SetHealth(Player.GetHealth() - 10);
			AttackDamageDelayTimer = 0.0f;
		}
		Enemy->bIsAttacking = true;

	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bHasReachedPlayer"), false);
		Enemy->bIsAttacking = false;
		AttackDamageDelayTimer = 0.0f;
	}
}

void AEnemyBTAIController::SetTargetPlayer()
{
	bool HasSeenPlayer1 = false;
	bool HasSeenPlayer2 = false;

	HasSeenPlayer1 = HasSeenPlayer(*Player1Pawn);
	HasSeenPlayer2 = HasSeenPlayer(*Player2Pawn);
	
	if (HasSeenPlayer1 || HasSeenPlayer2)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bIsPlayerSeen"), true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bIsPlayerSeen"), false);
	}

	if (HasSeenPlayer1 && !HasSeenPlayer2)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("ClosestPlayerActor"), Player1Pawn);
		bIsPlayer1Target = true; //bool used to know to attack 
	}
	else if (!HasSeenPlayer1 && HasSeenPlayer2)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("ClosestPlayerActor"), Player2Pawn);
		bIsPlayer1Target = false;
	}
	else if (HasSeenPlayer1 && HasSeenPlayer2) //Both players spotted, pick the closest one
	{
		if (Enemy->GetDistanceTo(Player1Pawn) < Enemy->GetDistanceTo(Player2Pawn))
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("ClosestPlayerActor"), Player1Pawn);
			bIsPlayer1Target = true;
		}
		else
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("ClosestPlayerActor"), Player2Pawn);
			bIsPlayer1Target = false;
		}
	}
}
