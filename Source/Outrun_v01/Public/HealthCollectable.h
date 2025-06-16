// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "HealthCollectable.generated.h"

/**
 * The HealthCollectable class is a child of the Collectable class and inherits the
 * OnOverlapBegin and OnOverlapEnd.
 * In addition, it uses the float HealingAmount to set a value for the player characters
 * health to increas when they overlap with one of the collectables.
 */
UCLASS()
class OUTRUN_V01_API AHealthCollectable : public ACollectable
{
	GENERATED_BODY()
	

public:

	AHealthCollectable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	float HealingAmount;

	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;

	virtual void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) override;

protected:

private:

};
