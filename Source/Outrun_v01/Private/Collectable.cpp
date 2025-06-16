// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
/*
* In the Collectable constructor, we are adding a default scene root and static mesh to any
* blueprints which are made from this class. Subsequently, any children of this class will use
* these parameters for their own collision detection, which reduces the need to set
* these parameters in multiple classes.
*/

ACollectable::ACollectable()
{

	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CollectableSceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMeshComponent"));

	StaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	StaticMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	OnActorBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACollectable::OnOverlapEnd);


}

void ACollectable::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void ACollectable::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	

}

void ACollectable::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}

void ACollectable::BeginPlay()
{

	Super::BeginPlay();

}

