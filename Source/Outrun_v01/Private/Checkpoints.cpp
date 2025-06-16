// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoints.h"
#include "MainCharacter.h"
#include "Engine/World.h"

/*
* The constructor assigns the default scene root and static mesh to instances of the checkpoint
* actors.
* In addition, it adds the dynamic OnOverlapBegin to the OnActorBeginOverlap variable.
* As we want to restore the player to full health of respawning, the value of FullHealth is set
* to the maximum for the player's health.
*/

ACheckpoints::ACheckpoints()
{

	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CollectableSceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMeshComponent"));

	StaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	StaticMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	OnActorBeginOverlap.AddDynamic(this, &ACheckpoints::OnOverlapBegin);

	FullHealth = 100.f;

}

/*
* OnOverlapBegin checks to see if an instance of MainCharacter overlaps with the actor.
* If it does, it adds the position of the checkpoint to the map against that particular
* instance of the player. PlayerCheckpointPos takes in an instance of MainCharacter
* and the 
*/

void ACheckpoints::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

	if (AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor))
	{


		PlayerCheckpointPos.FindOrAdd(MainChar) = GetActorLocation();
		

	}

	Super::OnOverlapBegin(OverlappedActor, OtherActor);

}

/*
* In the event that the player's health reaches zero, the TeleportPlayer function is called
* which will set the actor's location for that player back to the last checkpoint they overlapped.
* In addition, their health is set to full.
*/

void ACheckpoints::TeleportPlayer(AMainCharacter* MainChar)
{

	if (!MainChar || MainChar->GetHealth() > 0)
	{

		return;

	}

	if (FVector* CheckpointPos = PlayerCheckpointPos.Find(MainChar))
	{

		MainChar->SetActorLocation(*CheckpointPos);
		MainChar->SetHealth(FullHealth);


	}

}

/*
* The tick function continuously checks to see if the player needs to be teleported and call
* the TeleportPlayer function with the appropriate key for the map.
*/

void ACheckpoints::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	for (auto& PlayerEntry : PlayerCheckpointPos)
	{

		TeleportPlayer(PlayerEntry.Key);
	}

}
