// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "Checkpoints.generated.h"

/**
 * The Checkpoints class inherits from the GameManager class, to make use of the overridden
 * OnoverlapBegin class.
 * In this class, it will check to see if the player has overlapped with the checkpoint actor
 * and save the player's location locally.
 * In the event that the player's health reaches zero, it will teleport them back to the
 * last checkpoint that they overlapped.
 * In the actual game scene, the first instance of the Checkpoint actor should be set at the
 * end of the level and the newest actor should be the first.
 * It uses a map system with an instance of MainCharacter to store the player's position within a vector and
 * calls it when the teleportplayer function is called.
 */

UCLASS()
class OUTRUN_V01_API ACheckpoints : public AGameManager
{
	GENERATED_BODY()

public:

	ACheckpoints();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	USceneComponent* DefaultSceneRoot;

	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;
	
	void TeleportPlayer(AMainCharacter* MainChar);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	bool bHitDetected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	float FullHealth;

	AMainCharacter* TrackedPlayer;

private:

	TMap<AMainCharacter*, FVector> PlayerCheckpointPos;

};

