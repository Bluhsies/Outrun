// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "Collectable.generated.h"

/**
 * The Collectable class inherits from GameManager.
 * This class sets the static mesh for all children and adds the dynamic for OnOverlapBegin
 * and OnOverlapEnd.
 */
UCLASS()
class OUTRUN_V01_API ACollectable : public AGameManager
{
	GENERATED_BODY()

public:

	ACollectable();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	USceneComponent* DefaultSceneRoot;

	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;
	virtual void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) override;

protected:

	virtual void BeginPlay() override;


private:

	

	
};
