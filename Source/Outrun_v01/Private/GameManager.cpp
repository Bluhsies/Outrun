// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"



AGameManager::AGameManager()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* As these are virtual functions which will be overidden by child classes, no data needs to be
* set at this point.
*/

void AGameManager::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
}

void AGameManager::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}

void AGameManager::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

