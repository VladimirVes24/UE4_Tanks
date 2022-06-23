// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShooterActor.h"

// Sets default values
ABaseShooterActor::ABaseShooterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ABaseShooterActor::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ABaseShooterActor::DamageTaken);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body mesh"));
	RootComponent = BodyMesh;

	BodyCollisionBox = CreateDefaultSubobject<UBoxComponent>("Body collision");
	BodyCollisionBox->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower mesh"));
	TowerMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	TowerCollisionBox = CreateDefaultSubobject<UBoxComponent>("Tower collision");
	TowerCollisionBox->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	CannonCollisionBox = CreateDefaultSubobject<UBoxComponent>("Cannon Collision");
	CannonCollisionBox->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
}

// Called when the game starts or when spawned
void ABaseShooterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseShooterActor::Destroyed()
{
	if (Cannon) Cannon->Destroy();
}

void ABaseShooterActor::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ABaseShooterActor::Die()
{
	Destroy();
}

void ABaseShooterActor::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("%s took damage:%f  Current health:%f"), *GetName(),
	DamageValue, HealthComponent->GetHealth());
}

void ABaseShooterActor::SetupCannon(TSubclassOf<ACannon> DesiredCannonType)
{
	if (DesiredCannonType) { CannonType = DesiredCannonType; }
	if (Cannon) { Cannon->Destroy(); }
	
	const auto Transform = CannonSetupPoint->GetComponentTransform();
	FActorSpawnParameters CannonSpawnParams;
	CannonSpawnParams.Instigator = Cast<APawn>(this);
	CannonSpawnParams.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Transform, CannonSpawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called every frame
void ABaseShooterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



