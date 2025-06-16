// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "MainCharacter.h"
#include "Hazard.generated.h"

/**
 * The Hazard class derives from the GameManager class, to make use of the OnHit overidden function.
 * The class generates its own default scene root via code, as well as handling time for
 * the instances of damage to be applied to the player.
 */
UCLASS()
class OUTRUN_V01_API AHazard : public AGameManager
{
	GENERATED_BODY()
	
public:

	AHazard();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY()
	bool bHitDetected;

	UPROPERTY()
	FTimerHandle TimerHandle;

	void ResetHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
