// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "Enemy.h"

// Sets default values
APlayerProjectile::APlayerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));
	}

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("PlayerProjectile"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 1500.0f;
		ProjectileMovementComponent->MaxSpeed = 1500.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh (TEXT("'/Engine/BasicShapes/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
			ProjectileMeshComponent->SetCollisionProfileName((TEXT("PlayerProjectile")));
		}

		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Assets/Models_Animations/GunAssets/Bullets_M.Bullets_M'"));
		if (Material.Succeeded())
		{
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
			ProjectileMaterialInstance->SetVectorParameterValue(FName(TEXT("Base Color")), FLinearColor(0.5f,0.5f,0.0f));
		}
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	bIsInUse = false;
	ProjectileMovementComponent->Deactivate();
	SetActorLocation(GetOwner()->GetActorLocation());

	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor); Enemy)
	{
		Enemy->SetHealth( Enemy->GetHealth() - Damage );
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Green, FString::FormatAsNumber(Enemy->GetHealth()));
	}

}

void APlayerProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Activate();
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	bIsInUse = true;
}

