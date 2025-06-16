// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelManager::ALevelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelManager::OverlapBegins);
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	populateChamberSequence();
}


// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelManager::populateChamberSequence()
{
	//add a random number for each chamber
	//Tutorial is already in the level, so we ignore that
		//0 = Back_And_Forth
		//1 = Branching_Tree
		//2 = Huge_Arena
		//3 = Small_Staircases

	//select a seed, use time so it's always random 
	srand((unsigned)time(NULL));

	for (size_t i = 0; i < sizeof(chamberSequence); i++)
	{
		//Generate a number between 0 and amount of chambers
		int randomChamber = rand() % chamberAmount;

		//Add to array
		chamberSequence[i] = randomChamber;

		//Print to check
		UE_LOG(LogTemp, Warning, TEXT("chamberSequence[i] = %d"), chamberSequence[i]);
	}

	
}

void ALevelManager::enableChamber(int8 difficulty)
{

}

void ALevelManager::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == MyCharacter && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}
}