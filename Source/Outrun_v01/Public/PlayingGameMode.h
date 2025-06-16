// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BulletObjectPool.h"
#include "MainCharacter.h"
#include "PlayingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUTRUN_V01_API APlayingGameMode : public AGameMode
{
	GENERATED_BODY()
	
	void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABulletObjectPool> ObjectPoolClass;
};
