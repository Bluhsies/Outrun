// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "HealthCollectable.h"
#include "ScoreCollectable.h"
#include "Hazard.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "SaveMaster.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad.generated.h"

/**
 * The SaveLoad class inherits from the GameManager, which in turn inherits from the Actor class.
 * This class is used to collect the data from the game scene and store it within a series of
 * arrays/objects that derive from the actors in the scene which need components to be saved as
 * part of the save load system.
 * Additionally, it creates a custom save name and slot for the SaveGameToSlot and LoadGameFromSlot
 * functions in combination with the SaveMaster class and UGameplayStatics.
 */

UCLASS()
class OUTRUN_V01_API ASaveLoad : public AGameManager
{
	GENERATED_BODY()
	
public:
	
	ASaveLoad();

public:

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserSlot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString CollectableName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 CollectableSlot;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectables")
	TArray<AHealthCollectable*> HealthCollectableArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectables")
	TArray<AScoreCollectable*> ScoreCollectableArray;

	/*
	* The instances of AMainCharacter are setup different to the other containers, as there is
	* only one of each of these Actors. Whilst it is still possible to have an array for these,
	* the array would only contain two objects where as the other arrays will house multiple instances
	* of actor data which needs to be saved.
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	AMainCharacter* PlayerOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	AMainCharacter* PlayerTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazards")
	TArray<AHazard*> HazardArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TArray<AEnemy*> EnemyArray;

};
