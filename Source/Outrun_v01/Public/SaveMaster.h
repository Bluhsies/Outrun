// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveMaster.generated.h"

/*
* The SaveMaster class is used to create the save file for the game, which allows the user
* to save and load data.
* Through a number of structs and subsequent arrays setup in the class, it is used in conjunction
* with the SaveLoad script to save each individual instance of an actor (player, enemy, hazard, collectable)
* and load their associated data when the load function is called in SaveLoad.
* In addition, this also create the PlayerName and UserSlot variables, which are used to give
* the save game file individuality and that it would be overwritten when it is saved again.
* The save file can be located in an executable version within the savedgames folder.
*/

USTRUCT()
struct FActorTransforms
{

	GENERATED_BODY()

	UPROPERTY()
	FVector ActorPosition;

	UPROPERTY()
	FRotator ActorRotation;

};

USTRUCT()
struct FMoveableActorTransforms
{

	GENERATED_BODY()

	UPROPERTY()
	FVector MoveableActorPosition;

	UPROPERTY()
	FRotator MoveableActorRotation;

	UPROPERTY()
	float Health;

	UPROPERTY()
	float Score;

};

UCLASS()
class OUTRUN_V01_API USaveMaster : public USaveGame
{
	GENERATED_BODY()

public:

	USaveMaster();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserSlot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString CollectableName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 CollectableSlot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Health;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Score;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float MaxScore;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FActorTransforms> ActorPosRot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FActorTransforms> ColActorPosRot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FActorTransforms> HazActorPosRot;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FMoveableActorTransforms PlayerOneData;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FMoveableActorTransforms PlayerTwoData;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FActorTransforms> EnemyPosRot;

};
