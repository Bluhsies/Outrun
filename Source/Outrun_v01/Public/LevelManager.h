// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "LevelManager.generated.h"


UCLASS()
class OUTRUN_V01_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TEST TEST TEST
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Populate generation sequence with numbers representing chamber types
	void populateChamberSequence();

	void enableChamber(int8 difficulty);

	void disableChamber();


private:
	//Cap the size at 200, use int8 as it is extremely unlikely we exceed 127 chambers ever.
	int8 chamberSequence[200];

	//Use pointers so we don't need to waste time deleting and shifting the whole array around,
	//simply tick both pointers up when we're finished with a chamber. 
	int8* ptr_Activate;
	int8* ptr_Deactivate;

	//Set to amount of chambers (Excluding Tutorial) for modularity
	int8 chamberAmount = 4;

	// Overlap volume to trigger level streaming TEST TEST TEST
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;
};