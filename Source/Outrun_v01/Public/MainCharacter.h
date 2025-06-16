// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "PlayerProjectile.h"
#include "BulletObjectPool.h"
#include "GameUI.h"
#include "MainCharacter.generated.h"

class AGun;

class UNiagaraSystem;

UCLASS()
class OUTRUN_V01_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScore")
	float Score; 

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	//Mapping Context File + Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* UnCrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SwitchGunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ESCAction;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABulletObjectPool> ObjectPoolClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	class USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	class USoundBase* ShotgunShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	UNiagaraSystem* FireEffectMuzzle;


	UFUNCTION(BlueprintCallable) //Blueprint Callable for animation BP
	bool GetIsDashing();
	UFUNCTION(BlueprintCallable) //Blueprint Callable for animation BP
	bool GetSecondJump(); //Is the character on their second jump?
	UFUNCTION(BlueprintCallable) //Blueprint Callable for animation BP
	inline bool GetIsOnWall() { return IsOnWall; }
	
	//Health
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
	float MaxHealth;
	FORCEINLINE float GetHealth() { return Health; }
	void SetHealth(float Amount);
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float Amount) { MaxHealth = Amount; }

	//Score
	FORCEINLINE float GetScore() { return Score; }
	void SetScore(float Amount); 
	FORCEINLINE float GetMaxScore() { return MaxScore; } 
	FORCEINLINE void SetMaxScore(float Amount) { MaxScore = Amount; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerScore")
	float MaxScore; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UGameUI* HealthBarWidget;
	
	void ESCDown();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class APauseMenuPlayerController* PauseMenuController;

private:
	
	enum ShootingType
	{
		Standard,
		Machinegun,
		Shotgun
	};

	float PausedShootingTimes[3]; //Stores progress of cooldown timers for guns (pauses when no active shooting type)

	int8_t CurrentShootingType; //Stores currently selected shooting type
	int8_t NumberOfGunsUnlocked = 2;
	
	UCharacterMovementComponent* MovementComponent;
	USkeletalMeshComponent* CharacterMesh;
	
	float DashTimer = 0.0f;
	float ShootTimer = 0.0f;

	bool CanDash = true;
	bool CanCrouch = true;
	bool CanShoot = false;
	bool HasUndashed = true;
	bool IsOnWall = false; //Set in the GetIsOnWall function, which is called in Tick
	bool IsLeaningOnWall = false;
	bool WasTouchingWall = false;
	bool IsGunOffsetForCrouch = false;
	float WallTimer = 0.0f;

	const float DefaultBrakingDecelerationWalking = 2048.0f;
	const float DefaultBrakingFrictionFactor = 2.0f;
	const float DefaultGravityScale = 3.0f;

	int8 Direction = 1; //1 = Right, -1 = Left

	//Called for inputs
	void Move(const FInputActionValue& Value);
	void Jumping();
	void Crouching();
	void UnCrouching();
	void Dash(const FInputActionValue& Value);
	void StopDashing();
	void Shoot();
	void SwitchGun();
	void HandleAttachToWall(float DeltaTime);
	void OffsetMeshForColliderOnWall(int8 DirectionIn, float YOffset, float ZOffset); //Adjusting the mesh after actions to make sure it sits within capsule collider
	void OffsetMeshForColliderDashing(const FVector2D MovementVector);
	void OffsetMeshForColliderWalking(const FVector2D MovementVector);
	FVector GetMuzzleOffset(bool ActorOffset);
	bool ShootBullet(float Speed, int Damage, float ZOffset = 0.0f);
	void ShootShotgunBullets(float Speed, int Damage);
	float GetShootCooldown(int ShootingType);

	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;
	bool CheckEncroachment(); //Used to check if player can uncrouch after dashing
	bool TestAndGetIsOnWall();

	ABulletObjectPool* BulletObjectPool;
};
