// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.h"
#include "PlayerProjectile.h"
#include <vector>
#include "BulletObjectPool.generated.h"

UCLASS()
class OUTRUN_V01_API ABulletObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletObjectPool();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerProjectile> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APlayerProjectile* GetBulletNotInUse();

private:
	const int BulletCount = 15;
	std::vector<APlayerProjectile*> Bullets;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

};
