// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "ScoreCollectable.generated.h"

/**
 * 
 */
UCLASS()
class OUTRUN_V01_API AScoreCollectable : public ACollectable
{
	GENERATED_BODY()

public:

	AScoreCollectable();

	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float Score;

	

	

};
