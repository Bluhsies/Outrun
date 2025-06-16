// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Gun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameUI.h" 
#include "Blueprint/UserWidget.h" 
#include "PauseMenuPlayerController.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = GetCharacterMovement();
	CharacterMesh = GetMesh();

	Health = 85.f;
	MaxHealth = 100.f;

	Score = 0.f; 
	MaxScore = 10.f; 

	ShootSound = CreateDefaultSubobject<USoundBase>(TEXT("Shoot Sound"));
	ShotgunShootSound = CreateDefaultSubobject<USoundBase>(TEXT("Shotgun Shoot Sound"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PauseMenuController = Cast<APauseMenuPlayerController>(GetController());

	//Link PlayerController to input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0); //If we have a controller, and can find Enhanced Input subsystem, add mapping context variable to subsystem
		}
	}

	JumpMaxCount = 2;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
	Gun->SetOwner(this);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	BulletObjectPool = GetWorld()->SpawnActor<ABulletObjectPool>(ObjectPoolClass, SpawnParams);

	CurrentShootingType = ShootingType::Standard;

	//On game start, set cooldowns to not be zero so player dosen't have to wait when they haven't shot yet
	for (int i = 0; i < sizeof(PausedShootingTimes) / sizeof(PausedShootingTimes[0]); i++)
	{
		PausedShootingTimes[i] = GetShootCooldown(i);
	}

	if (HealthBarWidgetClass) 
	{ 
		HealthBarWidget = CreateWidget<UGameUI>(GetWorld(), HealthBarWidgetClass); 
		if (HealthBarWidget) 
		{ 
			HealthBarWidget->AddToViewport(); 
			HealthBarWidget->UpdateHealthBar(Health / MaxHealth, this); 
		} 
	} 

	 
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DashTimer += DeltaTime;
	ShootTimer += DeltaTime;

	if (!HasUndashed && DashTimer > 0.5 && GetIsDashing())
	{
		StopDashing();
	}

	HandleAttachToWall(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Bind Functions to call to actions
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AMainCharacter::Dash);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::Crouching);
		EnhancedInputComponent->BindAction(UnCrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::UnCrouching);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMainCharacter::Shoot);
		EnhancedInputComponent->BindAction(SwitchGunAction, ETriggerEvent::Triggered, this, &AMainCharacter::SwitchGun);
		EnhancedInputComponent->BindAction(ESCAction, ETriggerEvent::Started, this, &AMainCharacter::ESCDown);
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr && !GetIsDashing())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		//find out which way is forward based of of how the character is rotated
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (!IsLeaningOnWall)
		{
			AddMovementInput(RightDirection, MovementVector.X);
		}


		//Rotate towards velocity
		if (!GetIsDashing())
		{
			OffsetMeshForColliderWalking(MovementVector);
		}
	}
}

void AMainCharacter::Jumping()
{
	if (!GetIsDashing() && !TestAndGetIsOnWall())
	{
		Jump();
		WallTimer = 0.6f; //If player is stuck to wall and wants to jump off before stick timer runs down
	}
}

void AMainCharacter::Crouching()
{
	if (CanCrouch)
	{
		Crouch();
		//If under object, and crouch is pressed after being releeased, it still queues uncrouch, so force it keep queing crouch here
		MovementComponent->bWantsToCrouch = true;
	}
}

void AMainCharacter::UnCrouching()
{
	if (!GetIsDashing())
	{
		UnCrouch();
		CanDash = true;
	}
}

void AMainCharacter::Dash(const FInputActionValue& Value)
{
	if (CanDash && DashTimer > 1.0f && !TestAndGetIsOnWall())
	{
		DashTimer = 0.0f;
		const FVector2D MovementVector = Value.Get<FVector2D>();

		FVector LaunchVector = FVector(0, MovementVector.Y * 750, 0);

		if (MovementComponent->MovementMode == MOVE_Walking)
		{
			LaunchVector = FVector(0, MovementVector.Y * 1000, 0);
		}

		OffsetMeshForColliderDashing(MovementVector);

		Crouch();

		MovementComponent->Velocity = LaunchVector;
		MovementComponent->BrakingDecelerationWalking = 0;
		MovementComponent->BrakingFrictionFactor = 0;
		MovementComponent->GravityScale = 0;

		if (MovementComponent->MovementMode == MOVE_Falling)
		{
			CanDash = false;
		}

		MovementComponent->bWantsToCrouch = true;

		HasUndashed = false;
		Gun->SetVisibility(false);
		WallTimer = 0.6f;
	}
}

void AMainCharacter::StopDashing()
{
	MovementComponent->BrakingDecelerationWalking = DefaultBrakingDecelerationWalking;
	MovementComponent->BrakingFrictionFactor = DefaultBrakingFrictionFactor;
	MovementComponent->GravityScale = DefaultGravityScale;

	if (CheckEncroachment()) //Can uncrouch?
	{
		Crouch();
	}

	UnCrouching();
	HasUndashed = true;

	if (MovementComponent->MovementMode == MOVE_Falling)
	{
		CanDash = false;
	}
	Gun->SetVisibility(true);
}

void AMainCharacter::Shoot()
{
	if (CanShoot && BulletObjectPool)
	{	
		switch (CurrentShootingType)
		{
			case ShootingType::Standard:
			{
				ShootBullet(1500.0f, 25);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation(), 0.25f, FMath::RandRange( 0.9f, 1.1f));
				break;
			}
			case ShootingType::Machinegun:
			{
				ShootBullet(1000.0f, 17);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation(), 0.25f, FMath::RandRange(1.25f, 1.5f));
				break;
			}
			case ShootingType::Shotgun:
			{
				ShootShotgunBullets(1250.0f, 30);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunShootSound, GetActorLocation(), 0.25f, FMath::RandRange(0.9f, 1.1f), 0.5f);
				break;
			}
		}

		UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle, RootComponent, NAME_None, GetMuzzleOffset(false), FRotator(90, 0, 0), EAttachLocation::Type::KeepRelativeOffset, true);

		CanShoot = false;
		ShootTimer = 0.0f;
	}
	
	if (!IsOnWall && !GetIsDashing() && ShootTimer > GetShootCooldown(CurrentShootingType) )
	{
		CanShoot = true;
	}
}

void AMainCharacter::SwitchGun()
{
	PausedShootingTimes[CurrentShootingType] = ShootTimer;
	
	if (CurrentShootingType == NumberOfGunsUnlocked)
	{
		CurrentShootingType = 0;
	}
	else
	{
		CurrentShootingType++;
	}

	ShootTimer = PausedShootingTimes[CurrentShootingType]; //Store progress of shoot cooldown, which resumes when switching back
}

void AMainCharacter::HandleAttachToWall(float DeltaTime)
{
	if (MovementComponent->MovementMode == MOVE_Falling)
	{
		bool WasDashing = GetIsDashing();
		
		if (TestAndGetIsOnWall() || WallTimer < 0.25f)
		{	
			if (GetIsDashing())
			{
				StopDashing();
			}

			WallTimer += DeltaTime;

			CanDash = true;
			
			MovementComponent->Velocity = FMath::VInterpConstantTo(MovementComponent->Velocity, FVector(0, 0, 0), DeltaTime, 4000);
			JumpCurrentCount = -1;
			//If we have just started touching wall, or unreal's uncrouch function has moved the mesh out of the collider
			if (IsOnWall && (!WasTouchingWall || CharacterMesh->GetRelativeLocation().Z != -130))
			{
				OffsetMeshForColliderOnWall(1, 10, -130);
			}
			
			if (IsOnWall)
			{
				Gun->SetVisibility(false);
				IsLeaningOnWall = false;
			}
			else
			{
				Gun->SetVisibility(true);
				IsLeaningOnWall = true;
			}
		}
		else if(WallTimer > 0.25f)
		{
			IsLeaningOnWall = false;
			if (!IsOnWall && WasTouchingWall)
			{
				OffsetMeshForColliderOnWall(-1, 20, -90);
				Gun->SetVisibility(true);
				WallTimer = 0.0f;
			}
		}
	}
	else //Character is grounded, if was hanging on last frame then set offset so mesh is above ground
	{
		IsOnWall = false;
		IsLeaningOnWall = false;
		if (WasTouchingWall)
		{
			OffsetMeshForColliderOnWall(-1, 20, -90);
			Gun->SetVisibility(true);
		}
	}

	WasTouchingWall = IsOnWall;
}

void AMainCharacter::OffsetMeshForColliderOnWall(int8 DirectionIn, float YOffset, float ZOffset)
{
	if (Direction == DirectionIn)
	{
		CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, YOffset, ZOffset));
	}
	else
	{
		CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, -YOffset, ZOffset));
	}
}

void AMainCharacter::OffsetMeshForColliderDashing(const FVector2D MovementVector)
{
	if (MovementVector.Y == 1 && Direction != MovementVector.Y)
	{
		//If player has changed direction, rotate and move slightly to keep mesh nicely in collider
		CharacterMesh->SetRelativeRotation(FQuat(0, 0, 0, 0));
		CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, -20, CharacterMesh->GetRelativeLocation().Z));
		Direction = MovementVector.Y;
	}
	else if (Direction != MovementVector.Y)
	{
		CharacterMesh->SetRelativeRotation(FQuat(0, 0, 180, 0));
		CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, 20, CharacterMesh->GetRelativeLocation().Z));
		Direction = MovementVector.Y;
	}
}

void AMainCharacter::OffsetMeshForColliderWalking(const FVector2D MovementVector)
{
	if (MovementVector.X == 1 && Direction != MovementVector.X)
	{
		//If player has changed direction, rotate and move slightly to keep mesh nicely in collider
		CharacterMesh->SetRelativeRotation(FQuat(0, 0, 0, 0));
		if (MovementComponent->IsCrouching())
		{
			CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, -20, -62));
		}
		else
		{
			CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, -20, -90));
		}

		Direction = MovementVector.X;
	}
	else if (Direction != MovementVector.X)
	{
		CharacterMesh->SetRelativeRotation(FQuat(0, 0, 180, 0));
		if (MovementComponent->IsCrouching())
		{
			CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, 20, -62));
		}
		else
		{
			CharacterMesh->SetRelativeLocation(FVector(CharacterMesh->GetRelativeLocation().X, 20, -90));
		}

		Direction = MovementVector.X;
	}
}

FVector AMainCharacter::GetMuzzleOffset(bool ActorOffset)
{
	FVector Location = FVector(0);

	if (MovementComponent->IsCrouching())
	{
		if (MovementComponent->Velocity.Length() > 0)
		{
			Location = FVector(0, 75 * Direction, 32);
		}
		else
		{
			Location = FVector(0, 47 * Direction, 18);
		}
	}
	else if (MovementComponent->MovementMode == MOVE_Walking)
	{
		if (MovementComponent->Velocity.Length() > 0)
		{
			Location = FVector(0, 85 * Direction, 47);
		}
		else
		{
			Location = FVector(0, 73 * Direction, 57);
		}
	}
	else if (MovementComponent->MovementMode == MOVE_Falling)
	{
		Location = FVector(0, 70 * Direction, 32);
	}

	if (ActorOffset)
	{
		Location += GetActorLocation();
	}

	return Location;
}

bool AMainCharacter::ShootBullet(float Speed, int Damage, float ZOffset)
{
	if (APlayerProjectile* Bullet = BulletObjectPool->GetBulletNotInUse(); Bullet)
	{
		Bullet->SetDamage(Damage);
		Bullet->SetActorLocation(GetMuzzleOffset(true));
		Bullet->ProjectileMovementComponent->Activate();
		Bullet->ProjectileMovementComponent->InitialSpeed = Speed;
		Bullet->ProjectileMovementComponent->MaxSpeed = Speed;
		Bullet->FireInDirection(FVector(0, Direction, ZOffset));
		return true;
	}
	return false;
}

void AMainCharacter::ShootShotgunBullets(float Speed, int Damage)
{
	if (ShootBullet(Speed, Damage))
	{
		if (ShootBullet(Speed, Damage, 0.25f))
		{
			ShootBullet(Speed, Damage, -0.25f);
		}
	}
}

float AMainCharacter::GetShootCooldown(int ShootingType)
{
	switch (ShootingType)
	{
		case ShootingType::Standard:
		{
			return 0.5f;
		}
		case ShootingType::Machinegun:
		{
			return 0.25f;
		}
		case ShootingType::Shotgun:
		{
			return 1.5f;
		}
	}
	
	return 10.0f; //Makes obvious something has gone wrong here
}

bool AMainCharacter::GetIsDashing()
{
	if (MovementComponent != nullptr && MovementComponent->GravityScale <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AMainCharacter::GetSecondJump()
{
	if (MovementComponent != nullptr && JumpCurrentCount > 1 && MovementComponent->MovementMode == MOVE_Falling)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AMainCharacter::SetHealth(float Amount)
{
	if (Amount > MaxHealth)
	{
		Health = MaxHealth;
	}
	else if (Amount < 0)
	{
		Health = 0;
	}
	else
	{
		Health = Amount;
	}

	if (HealthBarWidget)
	{ 
		HealthBarWidget->UpdateHealthBar(Health / MaxHealth, this); 
	} 
}

void AMainCharacter::SetScore(float Amount)
{ 
	if (Amount < 0)
	{
		Score = 0;
	} 
	else 
	{ 
		Score = Amount; 
	} 

	if (HealthBarWidget)
	{

		HealthBarWidget->UpdateScore(this);

	}
} 

void AMainCharacter::ESCDown() 
{ 
	if (PauseMenuController) 
	{ 
		PauseMenuController->TogglePauseMenu(); 
	} 
} 

void AMainCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCustomMode);

	if (MovementComponent->MovementMode == MOVE_Walking)
	{
		CanDash = true;
		CanCrouch = true;
	}
	else if (MovementComponent->MovementMode == MOVE_Falling && !GetIsDashing())
	{
		CanCrouch = false;
		MovementComponent->GravityScale = 3;
		// If this is the first jump and we're already falling,
		// then unincrement the JumpCount to uncompensate for the built in unreal jump code
		const bool bFirstJump = JumpCurrentCount == 0;
		if (bFirstJump && MovementComponent->IsFalling())
		{
			JumpCurrentCount--;
		}
	}
}

//After dashing is finished, use this to check is character should uncrouch / grow hitbox
bool AMainCharacter::CheckEncroachment()
{
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ActorsToIgnore.Add(GetOwner());
	FVector TraceEnd = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 80);
	FHitResult HitResult;
	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), TraceEnd, 50.0f, ObjectTypes, 
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red, 2.0f);

	return Hit;
}

bool AMainCharacter::TestAndGetIsOnWall()
{
	//PLace box trace over hands to see if player should grab wall
	if (MovementComponent->MovementMode == MOVE_Falling)
	{
		TArray<AActor*> ActorsToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ActorsToIgnore.Add(GetOwner());
		FVector TraceStart = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 40);
		FVector TraceEnd = FVector(GetActorLocation().X, GetActorLocation().Y + 40, GetActorLocation().Z + 40);

		if (Direction == -1)
		{
			TraceEnd = FVector(GetActorLocation().X, GetActorLocation().Y - 40, GetActorLocation().Z + 40);
		}

		FVector HalfSize = FVector(20.f, 20.f, 20.f);
		FHitResult HitResult;
		const bool Hit = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, HalfSize, FRotator(0, 0, 0), ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

		IsOnWall = Hit;

		return Hit;
	}

	return false;
}

