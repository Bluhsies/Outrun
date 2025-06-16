// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletObjectPool.h"

// Sets default values
ABulletObjectPool::ABulletObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

}

// Called when the game starts or when spawned
void ABulletObjectPool::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < BulletCount; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		APlayerProjectile* Projectile = GetWorld()->SpawnActor<APlayerProjectile>(ProjectileClass, SpawnParams);
		if (Projectile)
		{
			Projectile->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			Projectile->ProjectileMovementComponent->Deactivate();
		}
		Bullets.push_back(Projectile);
	}
}

// Called every frame
void ABulletObjectPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APlayerProjectile* ABulletObjectPool::GetBulletNotInUse()
{
	for (APlayerProjectile* Bullet : Bullets)
	{
		if (!Bullet->bIsInUse)
		{
			return Bullet;
		}
	}

	return nullptr;

}
