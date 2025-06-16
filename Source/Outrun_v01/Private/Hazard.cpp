// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"

/*
* The constructor will setup the default scene root and static mesh for the hazard
* blueprint actor, which is generated from this class.
* In addition, it allows you to set the value for damage that the player will take and set bHitDetected
* to false.
* The DamageAmount variable can be changed in the inspector locally for each instance of the hazard
* actor.
*/

AHazard::AHazard()
{

	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	StaticMesh->BodyInstance.SetCollisionProfileName(TEXT("Hazard"));

	StaticMesh->BodyInstance.SetInstanceNotifyRBCollision(true);

	StaticMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	StaticMesh->OnComponentHit.AddDynamic(this, &AHazard::OnHit);

	DamageAmount = 35.f;

	bHitDetected = false;

}

void AHazard::ResetHit()
{

	bHitDetected = false;

}

void AHazard::BeginPlay()
{

	Super::BeginPlay();

}

void AHazard::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

/*
* The OnHit function is overidden from the GameManager class.
* This function checks to see if an instance of the MainCharacter collides with the
* hazard actor in the scene. If it does, it will check to see if a hit is detected and reduce
* the player's health by the value assigned in DamageAmount.
* Using a timer, it will prevent the player from taking damage more than once a second if they stand close by.
* Ideally, it should allow the player to touch it once and move away, unless they purposefully want to
* kill their character.
*/

void AHazard::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (Cast<AMainCharacter>(OtherActor))
	{

		GetWorldTimerManager().ClearTimer(TimerHandle);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AHazard::ResetHit, 1.0f, false, 0.1f);

		if (bHitDetected == false)
		{

			GLog->Log("Character Hit Detected");
			AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);
			MainChar->SetHealth(MainChar->GetHealth() - DamageAmount);
			
			GLog->Log(FString::SanitizeFloat(MainChar->GetVelocity().Length()));

			bHitDetected = true;
		}
	}

}
